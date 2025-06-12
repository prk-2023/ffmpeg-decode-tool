#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libavutil/error.h>
#include <libavutil/md5.h>
}

#include <sys/select.h> // for select(), fd_set
#include <sys/time.h>   // for timeval
#include <fcntl.h>      // for file control options (not used here but often helpful)

#define SEEK_STEP 5 // seconds
const char* loglevel = nullptr; // DEBUG Level

enum DecoderType {
   SOFTWARE,
   HARDWARE
};

struct termios originalTermSettings;
// Function to save the original terminal settings
void saveTerminalSettings() {
   tcgetattr(STDIN_FILENO, &originalTermSettings); // Save the current terminal settings
}

// Function to restore terminal settings to their original state
void restoreTerminalSettings() {
   tcsetattr(STDIN_FILENO, TCSANOW, &originalTermSettings); // Restore original settings
}

void cleanupKeyboard() {
   restoreTerminalSettings(); // Restore terminal settings on cleanup
}

class FFmpegDemuxSeeker {
   public:
      FFmpegDemuxSeeker(const std::string& filename, DecoderType decoder_type , const std::string& codecName = nullptr) 
         : decoder_type(decoder_type), fmt_ctx(nullptr), codec_ctx(nullptr), video_stream_index(-1),
         current_pos(0), duration(0), frame_number(0), 
         quit_flag(false), seek_requested(false), seek_offset(0)
   {
      // debug levels  av_log_set_level(AV_LOG_INFO); default
      // AV_LOG_QUIE; // no output
      // AV_LOG_PANIC // for unrecoverable errors that can cause prog to crash
      // AV_LOG_FATAL // serious errors that might stop the program.
      // AV_LOG_ERROR // Standard error messages (e.g., decoding failure).
      // AV_LOG_WARNING // Warnings (recoverable problems).
      // AV_LOG_INFO  // basic info
      // AV_LOG_VERBOSE  // more then basic info
      // AV_LOG_DEBUG  // Extremely detailed debug information.
      // AV_LOG_TRACE  // All messages, including very low-level function calls.
      // Trace should print 
      // - decoder state transitions
      // - frame timestamps
      // - HW acceleratio"info"ls 
      // - Internal buffer allocation information 

      if (loglevel) {
         printf(" Debug level: %s \n", loglevel);
         if (strncmp(loglevel, "trace", 5) == 0) {  
            av_log_set_level(AV_LOG_TRACE); 
         } else if (strncmp(loglevel, "info", 4) == 0) {
            av_log_set_level(AV_LOG_INFO); 
         } else if (strncmp(loglevel, "debug", 5) == 0) {
            av_log_set_level(AV_LOG_DEBUG); 
         } else {
            av_log_set_level(AV_LOG_INFO); 
         }
      } else {
         av_log_set_level(AV_LOG_INFO);  // Default log level
      }
      if (avformat_open_input(&fmt_ctx, filename.c_str(), nullptr, nullptr) < 0)
         throw std::runtime_error("Failed to open file");

      if (avformat_find_stream_info(fmt_ctx, nullptr) < 0)
         throw std::runtime_error("Failed to find stream info");

      for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
         if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
         }
      }

      if (video_stream_index == -1)
         throw std::runtime_error("No video stream found");

      const AVCodec *codec = nullptr;
      if (decoder_type == SOFTWARE) { 
         codec = avcodec_find_decoder(fmt_ctx->streams[video_stream_index]->codecpar->codec_id);
      } else if ( decoder_type == HARDWARE) {
         // codec = avcodec_find_decoder_by_name("h264_v4l2m2m");
         codec = avcodec_find_decoder_by_name(codecName.c_str());
         if (!codec) {
            std::cerr << "[Error] v4l2_m2m decoder not found!!!";
            //fallback
            ////codec = avcodec_find_decoder(fmt_ctx->streams[video_stream_index]->codecpar->codec_id); 
            throw std::runtime_error("Error to open HW codec!!!!");
         }
      }
      if (!codec)
         throw std::runtime_error("Unsupported codec");

      codec_ctx = avcodec_alloc_context3(codec);
      if (codec_ctx) {
         codec_ctx->flags |= AV_CODEC_FLAG_OUTPUT_CORRUPT; // TODO need to check if this is true for HW
                                                           // decode
         codec_ctx->flags2 |= AV_CODEC_FLAG2_SHOW_ALL;
         //Force errors to be visible ( SW decode only)
         if ( decoder_type == SOFTWARE ) { 
            // confirmed the below flags are used only by FFMpeg SW decoders for error detection
            // on 1619 : HW decoding does not support them ( instead use is_hw_frame_corrupt() below)
            codec_ctx->err_recognition = 
               AV_EF_CAREFUL   | 
               AV_EF_CRCCHECK  |
               AV_EF_BITSTREAM | //bitstream errors
               AV_EF_BUFFER    | // check buffer boundaries
               AV_EF_EXPLODE    //aborts on error ( can not conceal )
               ;
            codec_ctx->debug = FF_DEBUG_MB_TYPE | FF_DEBUG_SKIP;// macroblock works SW H.264
         }
      }

      avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);
      if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
         throw std::runtime_error("Failed to open codec");

      duration = fmt_ctx->duration;
      std::cout << "Loaded: " << filename << ", duration: " << (duration / AV_TIME_BASE) << " sec\n";
      // Print general format-level info
      std::cout << "Input file: " << fmt_ctx->url << "\n";
      AVCodecParameters* codecpar = nullptr;
      AVStream* video_stream = nullptr;
      int video_stream_index = -1;

      // Find the best video stream
      for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i) {
         if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = fmt_ctx->streams[i];
            codecpar = video_stream->codecpar;
            video_stream_index = i;
            break;
         }
      }
      if (!video_stream || !codecpar)
         throw std::runtime_error("No video stream found");
      const char* codec_long_name = codec ? codec->long_name : "unknown";

      // Duration (in seconds)
      double duration_sec = (fmt_ctx->duration != AV_NOPTS_VALUE)
         ? fmt_ctx->duration * av_q2d(AV_TIME_BASE_Q)
         : 0;

      // Bitrate (in kbps)
      int64_t bitrate = fmt_ctx->bit_rate;

      std::cout << "Video stream index: " << video_stream_index << "\n";
      std::cout << "Encoded format: " << codec_long_name << "\n";
      std::cout << "Codec ID: " << codecpar->codec_id << "\n";
      std::cout << "Resolution: " << codecpar->width << "x" << codecpar->height << "\n";
      std::cout << "Pixel format: " << av_get_pix_fmt_name((AVPixelFormat)codecpar->format) << "\n";
      std::cout << "Duration: " << duration_sec << " seconds\n";
      std::cout << "Overall Bitrate:(includes all streams) " << (bitrate / 1000) << " kbps\n";
      std::cout << "Video stream Bitrate: " << (codecpar->bit_rate / 1000) << " kbps\n";
      std::cout << "Decoder used : " << (codec->name) << "\n";

   }

      ~FFmpegDemuxSeeker() {
         if (codec_ctx)
            avcodec_free_context(&codec_ctx);
         if (fmt_ctx)
            avformat_close_input(&fmt_ctx);
      }

      void run() {
         std::thread demux_thread(&FFmpegDemuxSeeker::demuxLoop, this);
         std::thread input_thread(&FFmpegDemuxSeeker::inputLoop, this);

         demux_thread.join();
         input_thread.join();
      }

   private:
      DecoderType decoder_type;
      const char* codecStr = nullptr;
      AVFormatContext* fmt_ctx;
      AVCodecContext* codec_ctx;
      int video_stream_index;
      int64_t current_pos;
      int64_t duration;
      int64_t frame_number;

      std::atomic<bool> quit_flag;
      std::atomic<bool> seek_requested;
      std::mutex seek_mutex;
      int64_t seek_offset; // in microseconds

      void demuxLoop() {
         AVPacket* packet = av_packet_alloc();
         AVFrame* frame = av_frame_alloc();

         while (!quit_flag) {
            if (seek_requested) {
               std::lock_guard<std::mutex> lock(seek_mutex);
               int64_t new_pos = current_pos + seek_offset;
               if (new_pos < 0) new_pos = 0;
               if (new_pos > duration) new_pos = duration;
               current_pos = new_pos;

               int64_t ts = av_rescale_q(current_pos, AV_TIME_BASE_Q,
                     fmt_ctx->streams[video_stream_index]->time_base);
               if (av_seek_frame(fmt_ctx, video_stream_index, ts, AVSEEK_FLAG_BACKWARD) < 0) {
                  std::cerr << "[Seek] Failed\n";
               } else {
                  avcodec_flush_buffers(codec_ctx);
                  std::cout << "[Seek] Jumped to " << current_pos / AV_TIME_BASE << " sec\n";
               }

               seek_requested = false;
            }

            int ret = av_read_frame(fmt_ctx, packet);
            if (ret < 0) {
               if (ret == AVERROR_EOF) {
                  std::cout << "[EOF reached]\n";
                  quit_flag = true;
               } else {
                  char errbuf[AV_ERROR_MAX_STRING_SIZE];
                  std::cerr << "[Error reading frame: "
                     << av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret)
                     << "]\n";
               }
               break;
            }

            if (packet->stream_index == video_stream_index) {
               if (avcodec_send_packet(codec_ctx, packet) == 0) {
                  while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                     bool is_corrupt = false;
                     if (frame->flags & AV_FRAME_FLAG_CORRUPT || 
                           frame->decode_error_flags || 
                           (packet && (packet->flags & AV_PKT_FLAG_CORRUPT))
                        ) 
                     {
                        is_corrupt = true;
                        std::cout << "\n=== CORRUPTION DETECTED === \n";
                        std::cout << "\nPacket PTS: " << (packet ? packet->pts : -1);
                        std::cout << "\nFrame PTS: " << frame->pts;
                        std::cout << "\nError Flags: " << std::hex << frame->decode_error_flags << std::dec;
                        printFrameInfo(frame); // This will now show macroblock map
                     } else {
                        printFrameInfo(frame); // Normal frame output
                     }

                     // if(frame->flags & AV_FRAME_FLAG_CORRUPT) {
                     //    printf("[Warning] Corrupted frame detected at PTS %" PRId64 "\n", frame->pts);
                     // }
                     // // check if any part of frame (macroblocks) are affected
                     // if (packet->flags & AV_PKT_FLAG_CORRUPT) {
                     //    printf("[WARNING] Corrupted packet detected at PTS %" PRId64"\n", packet->pts);
                     // }
                     // // print frame info 
                     // printFrameInfo(frame);
                     if (is_hw_frame_corrupt(frame)) {
                        std::cerr << "[HW] Visual corruption detected (PTS: " << frame->pts << ")\n";
                        is_corrupt = true;
                     }
                  } //--
               }
            }
            av_packet_unref(packet);
            //usleep(3300); // reduce CPU usage ( set to 30 fps )
            usleep(9000); // reduce CPU usage ( set to 30 fps )
         }

         av_frame_free(&frame);
         av_packet_free(&packet);
      }

      void inputLoop() {
         std::cout << "Controls:\n"
            << "  s - Seek forward 5s\n"
            << "  a - Seek backward 5s\n"
            << "  q - Quit\n";

         while (!quit_flag) {
            //char c = getch(); // blocking 
            char c = getch_select(); //non-blocking
            if (c == 'q') {
               quit_flag = true;
               std::cout << "[Quit]\n";
               break;
            } else if (c == 's') {
               requestSeek(SEEK_STEP * AV_TIME_BASE);
            } else if (c == 'a') {
               requestSeek(-SEEK_STEP * AV_TIME_BASE);
            }
         }
      }

      void requestSeek(int64_t offset) {
         std::lock_guard<std::mutex> lock(seek_mutex);
         seek_offset = offset;
         seek_requested = true;
      }

      void printFrameInfo(AVFrame* frame) {
         // Initialize MD5 context
         AVMD5* md5 = av_md5_alloc();
         if (!md5) {
            std::cerr << "Failed to allocate MD5 context\n";
            return;
         }

         av_md5_init(md5);

         // Hash the pixel data
         // int frame_size = 0;
         for (int plane = 0; plane < AV_NUM_DATA_POINTERS && frame->data[plane]; plane++) {
            int linesize = frame->linesize[plane];
            int height = (plane == 0 || frame->format == AV_PIX_FMT_GRAY8) ? frame->height : frame->height / 2;

            for (int y = 0; y < height; y++) {
               av_md5_update(md5, frame->data[plane] + y * linesize, linesize);
               // frame_size += linesize; //accumulated size
            }
         }

         uint8_t digest[16];
         av_md5_final(md5, digest);
         av_free(md5);

         // Format MD5 as hex string
         char md5string[33];
         for (int i = 0; i < 16; i++) {
            snprintf(md5string + i * 2, 3, "%02x", digest[i]);
         }

         char pict_type_char = av_get_picture_type_char(frame->pict_type);

         if (frame->pts == AV_NOPTS_VALUE)
            std::cout << "[NOTICE] Frame missing PTS\n";
         
         if (frame->pict_type == AV_PICTURE_TYPE_NONE)
            std::cout << "[NOTICE] Frame picture type unknown\n";

         // uncomment the below  HW decoder missing PTS/DTS and we wish to fix this.
         // For this tool we do not fix (purpose is to raw comparision)
         // bool use_best_effort = true;  //TODO: controlled via config or CLI arg
         // int64_t raw_pts = frame->pts;
         // int64_t effective_pts = -1;
         // if (use_best_effort) {
         //   effective_pts = (frame->best_effort_timestamp != AV_NOPTS_VALUE)
         //           ? frame->best_effort_timestamp
         //           : (frame->pts != AV_NOPTS_VALUE ? frame->pts : -1);
         // } else {
         //      effective_pts = frame->pts;
         // }
         // double timestamp = (effective_pts != -1)
         //          ? effective_pts * av_q2d(fmt_ctx->streams[video_stream_index]->time_base)
         //          : -1;

         double timestamp = (frame->pts != AV_NOPTS_VALUE)
            ? frame->pts * av_q2d(fmt_ctx->streams[video_stream_index]->time_base)
            : -1;

         // int buffer_size = av_image_get_buffer_size((AVPixelFormat)frame->format,
         //                                         frame->width, frame->height, 1);

         const char* pix_fmt_name = av_get_pix_fmt_name(static_cast<AVPixelFormat>(frame->format));

         std::cout << "Frame #" << frame_number++
            << " | Type: " << pict_type_char
            << " | PTS: " << frame->pts
            << " | DTS: " << frame->pkt_dts
            << " | Timestamp: " << timestamp << "s"
            << " | Resolution: " << frame->width << "x" << frame->height
            //<< " | Frame Size: " << frame_size << " bytes"
            //<< " | Estimated Buffer Size: " << buffer_size << " bytes"
            << " | Pixel fmt: " << (pix_fmt_name ? pix_fmt_name : "unknown")
            << " | Decoded frm MD5: " << md5string
            << "\n";

         // Update the current_pos here
         if (frame->pts != AV_NOPTS_VALUE) {
            current_pos = av_rescale_q(frame->pts,
                  fmt_ctx->streams[video_stream_index]->time_base,
                  AV_TIME_BASE_Q);
         }

         if (frame->decode_error_flags) {
            if (frame->decode_error_flags & FF_DECODE_ERROR_INVALID_BITSTREAM)
               std::cout << "[WARNING] Invalid Bitstream!!!! \n";
            if (frame->decode_error_flags & FF_DECODE_ERROR_MISSING_REFERENCE)
               std::cout << "[WARNING] Missing Reference!!!! \n";
            if (frame->decode_error_flags & FF_DECODE_ERROR_CONCEALMENT_ACTIVE) {
               std::cout << "[WARNING] Error Concealment Used!!!! \n";
               std::cout << "[Concealment Active] PTS: " << frame->pts << "\n";
            }
         }
      }

      /* Visual artifact detection in frame buffer 
       * Analyze decoded Y/U/V planes for 
       * 1. Solid colors 
       *    Y-plane solid check detects black/green/solid output typical of decoder error Concealment 
       * 2. Flat regions 
       *    UV-plane flatness check identifies gray/green block corruption (e.g. U=128, V=128 uniform) 
       * 3. Dead Pixels or strips 
       *
       */
      bool is_hw_frame_corrupt(AVFrame* frame) {
         // Check known HW output formats 
         if (frame->format == AV_PIX_FMT_NV12) {
            // --- Solid Y plane check (black/green/pink screen, etc.)
            if (frame->data[0] && frame->linesize[0] > 0) {
               const uint8_t reference = frame->data[0][0];
               const int stride = frame->linesize[0];
               const int height = frame->height;

               int solid_count = 0;
               int sample_lines = std::min(10, height);  // sample only a few lines

               for (int y = 0; y < sample_lines; ++y) {
                  uint8_t* row = frame->data[0] + y * stride;
                  bool uniform_line = true;

                  for (int x = 1; x < frame->width; ++x) {
                     if (row[x] != reference) {
                        uniform_line = false;
                        break;
                     }
                  }

                  if (uniform_line)
                     solid_count++;
               }

               if (solid_count == sample_lines) {
                  // Very likely a corrupted frame (solid color)
                  return true;
               }
            }

            // Optional: Add U/V plane check for highly compressed color distortions
            // Example: check U and V planes for flat regions or 0x80 artifacts
            if (frame->format == AV_PIX_FMT_NV12 && frame->data[1]) {
               int uv_stride = frame->linesize[1];
               uint8_t* uv_data = frame->data[1];

               // Just check first row of UV interleaved
               const uint8_t u0 = uv_data[0];
               const uint8_t v0 = uv_data[1];
               bool uv_flat = true;

               for (int x = 2; x < frame->width; x += 2) {
                  if (uv_data[x] != u0 || uv_data[x + 1] != v0) {
                     uv_flat = false;
                     break;
                  }
               }

               if (uv_flat) {
                  // UV too flat → may be gray/green block artifact
                  return true;
               }
            }
         }

         return false;
      }
#if 0
      bool is_hw_frame_corrupt(AVFrame* frame) {
         // Check for solid-color frames in common HW formats
         // TODO : handle DRM_PRIME if required

         // if (frame->format == AV_PIX_FMT_DRM_PRIME || frame->format == AV_PIX_FMT_NV12) {
         if ( frame->format == AV_PIX_FMT_NV12) {
            // For NV12, check the Y plane (luma) for solid color
            if (frame->data[0] && frame->linesize[0] > 0) {
               const int sample_size = 10;
               const uint8_t first_pixel = frame->data[0][0];

               // Sample top-left corner pixels
               for (int i = 0; i < sample_size; i++) {
                  if (frame->data[0][i] != first_pixel) 
                     return false;
               }
               return true; // Likely corrupted (solid color)
            }
         }
         return false;
      }
#endif
      //blocking
      char getch() {
         char buf = 0;
         struct termios old = {0};
         tcgetattr(STDIN_FILENO, &old);
         old.c_lflag &= ~(ICANON | ECHO);
         tcsetattr(STDIN_FILENO, TCSANOW, &old);
         read(STDIN_FILENO, &buf, 1);
         tcsetattr(STDIN_FILENO, TCSANOW, &old);
         return buf;
      }

      // Non-blocking use select()
      // Returns: ascii value of character pressed, or -1 if no input within timeout
      char getch_select(int timeout_ms = 0) {
         struct termios oldt, newt;
         tcgetattr(STDIN_FILENO, &oldt);         // get current terminal settings
         newt = oldt;
         newt.c_lflag &= ~(ICANON | ECHO);       // disable canonical mode and echo
         tcsetattr(STDIN_FILENO, TCSANOW, &newt);// apply new settings immediately

         fd_set readfds;
         FD_ZERO(&readfds);
         FD_SET(STDIN_FILENO, &readfds);

         struct timeval tv;
         tv.tv_sec = timeout_ms / 1000;
         tv.tv_usec = (timeout_ms % 1000) * 1000;

         char ch = -1;
         int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL,
               (timeout_ms >= 0 ? &tv : NULL)); // NULL means wait indefinitely

         if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            char buf;
            if (read(STDIN_FILENO, &buf, 1) == 1) {
               ch = static_cast<unsigned char>(buf);
            }
         }
         tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore original terminal settings
         return ch;
      }
      };

      // Main
      int main(int argc, char* argv[]) {
         const char* inputFile = nullptr;
         const char* decoderStr = nullptr; //"SW";  // default
         const char* codecStr = nullptr;
         DecoderType decoder = SOFTWARE;

         int opt;
         while ((opt = getopt(argc, argv, "i:d:c:v:h")) != -1) {
            switch (opt) {
               case 'i':
                  inputFile = optarg;
                  break;
               case 'd':
                  decoderStr = optarg;
                  break;
               case 'c':
                  codecStr = optarg;
                  break;
               case 'v': 
                  loglevel = optarg;
                  break;
               case 'h':
               default:
                  std::cerr << "Usage: " << argv[0]
                     << " -i <input_file> [-d HW|SW] [-c <codec>]\n";
                  return 1;
            }
         }
         //check options
         if (!inputFile || !decoderStr || !codecStr) {
            std::cerr << "Error: Missing required options -i and/or -d\n";
            std::cerr << "Usage: " << argv[0]
               << " -i <input_file> -d <HW|SW> [-c <codec>]\n";
            std::cerr << "Usage: " << argv[0] << " -i <input_file> -d <DecoderType: HW|SW> [-c <if decoderType is HW: codec_name>]\n";
            std::cerr << "  ex1: " << argv[0] << " -i <input_file> -d HW -c hevc_v4l2m2m\n";
            std::cerr << "  ex2: " << argv[0] << " -i <input_file> -d SW -c auto [for SW decode use auto]\n";
            return 1;
         }
         // Validate decoder option and codec option
         if (strncmp(decoderStr, "HW", 2) == 0) {
            decoder = HARDWARE;
         } else if (strncmp(decoderStr, "SW", 2) == 0) {
            decoder = SOFTWARE;
         } else {
            std::cerr << "Error: Invalid decoder type '" << decoderStr
               << "'. Must be HW or SW.\n";

            return 1;
         }

         saveTerminalSettings();


         try {
            FFmpegDemuxSeeker demux_seeker(inputFile, decoder, codecStr);
            demux_seeker.run();
         } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
            cleanupKeyboard();
            return 1;
         }

         cleanupKeyboard();
         return 0;
      }
