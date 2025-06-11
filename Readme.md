# ffmpeg-decode-tool

ffmpeg based multithreaded utility:
- video demuxing and seeking tool 
- (SW/HW) decoding
- display frame info along with ERRORS.
- supports log levels for additional info ( trace, debug, info )


## main Features

* **Multithreaded Demuxing**: reads and decodes multimedia streams using multiple threads.
* **Precise Seeking**: Implements frame-accurate seeking to specific timestamps within video files.
                       Seek always moves to the neareast I frame.
* **Thread Synchronization**: Utilizes mutexes and condition variables to manage thread communication and synchronization.
* **FFmpeg Integration**: Leverages FFmpeg's libavformat and libavcodec libraries for media processing.

## Requirements

* FFmpeg libraries (libavformat, libavcodec, libavutil)
* C++11 or later
* make and CMake for building the project

   ```bash
   mkdir build
   cd build; cmake .. ; make
   ```

## Code Overview

* **Demuxing Thread**: Reads packets from the input file and pushes them to the appropriate decoder thread.
* **Video Decoder Thread**: Decodes video packets and processes them.
* **Seeking Mechanism**: Handles seeking requests by flushing codec buffers and adjusting playback position.
* reports decoded Frame Errors and Warnings

* Supports HW and SW decoding

usage ffmpeg_seeker -i <inputfile> -d <decoder type HW/SW> -c <codec_name> -v <log_level>

log_levels: trace, debug, info


example output:

```bash
root@bleedingedge-rtd1619b:/mnt/2025/06#  ffmpeg_seeker -i corrupted_bitstream.mp4 -d HW -c h264_v4l2m2m  -v info
 Debug level: info
[h264_v4l2m2m @ 0x555e924440] Using device /dev/video1
[h264_v4l2m2m @ 0x555e924440] driver 'realtek-vpu' on card 'realtek-vpu' in mplane mode
[h264_v4l2m2m @ 0x555e924440] requesting formats: output=H264/none capture=NV12/yuv420p
Loaded: corrupted_bitstream.mp4, duration: 311 sec
Input file: corrupted_bitstream.mp4
Video stream index: 0
Encoded format: V4L2 mem2mem H.264 decoder wrapper
Codec ID: 27
Resolution: 1920x1080
Pixel format: yuv420p
Duration: 311.428 seconds
Overall Bitrate:(includes all streams) 3982 kbps
Video stream Bitrate: 3725 kbps
Decoder used : h264_v4l2m2m
Controls:
  s - Seek forward 5s
  a - Seek backward 5s
  q - Quit
Frame #0 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #1 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #2 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #3 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #4 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #5 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #6 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #7 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #8 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #9 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #10 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #11 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #12 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #13 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #14 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #15 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #16 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #17 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #18 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #19 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #20 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #21 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #22 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #23 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #24 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: fa11a15cfa030a5b53acf34cfe070f3c
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #25 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 7f35be4dad65f2d78250dfe5e7a6f1b9
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #26 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 0d7b775c65c8e6343697eb30b4b5cdb3
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #27 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 65c321f7e0e3a5d70918270dbd8ea47e
[HW] Visual corruption detected (PTS: -9223372036854775808)
[Quit]
Frame #28 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: dc949c3cdafb5a04e464860134a1c5e7
[HW] Visual corruption detected (PTS: -9223372036854775808)
Frame #29 | Type: ? | PTS: -9223372036854775808 | DTS: -9223372036854775808 | Timestamp: -1s | Resolution: 1920x1080 | Pixel fmt: nv12 | Decoded frm MD5: 7bea306f8fda3f68dc4ba4a99e89ad22
[HW] Visual corruption detected (PTS: -9223372036854775808)
```

SW decoding
```bash
root@bleedingedge-rtd1619b:/mnt/2025/06# ffmpeg_seeker -i corrupted_bitstream.mp4 -d SW -c auto -v info  -v info
 Debug level: info 
Loaded: corrupted_bitstream.mp4, duration: 311 sec
Input file: corrupted_bitstream.mp4
Video stream index: 0
Encoded format: H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10
Codec ID: 27
Resolution: 1920x1080
Pixel format: yuv420p
Duration: 311.428 seconds
Overall Bitrate:(includes all streams) 3982 kbps
Video stream Bitrate: 3725 kbps
Decoder used : h264
Controls:
  s - Seek forward 5s
  a - Seek backward 5s
  q - Quit
Frame #0 | Type: I | PTS: 552 | DTS: 552 | Timestamp: 0.023s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #1 | Type: B | PTS: 1553 | DTS: 1553 | Timestamp: 0.0647083s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #2 | Type: B | PTS: 2554 | DTS: 2554 | Timestamp: 0.106417s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #3 | Type: P | PTS: 3555 | DTS: 3555 | Timestamp: 0.148125s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #4 | Type: B | PTS: 4556 | DTS: 4556 | Timestamp: 0.189833s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #5 | Type: B | PTS: 5557 | DTS: 5557 | Timestamp: 0.231542s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #6 | Type: P | PTS: 6558 | DTS: 6558 | Timestamp: 0.27325s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #7 | Type: B | PTS: 7559 | DTS: 7559 | Timestamp: 0.314958s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #8 | Type: B | PTS: 8560 | DTS: 8560 | Timestamp: 0.356667s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #9 | Type: P | PTS: 9561 | DTS: 9561 | Timestamp: 0.398375s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #10 | Type: B | PTS: 10562 | DTS: 10562 | Timestamp: 0.440083s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #11 | Type: B | PTS: 11563 | DTS: 11563 | Timestamp: 0.481792s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #12 | Type: P | PTS: 12564 | DTS: 12564 | Timestamp: 0.5235s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #13 | Type: B | PTS: 13565 | DTS: 13565 | Timestamp: 0.565208s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #14 | Type: B | PTS: 14566 | DTS: 14566 | Timestamp: 0.606917s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #15 | Type: P | PTS: 15567 | DTS: 15567 | Timestamp: 0.648625s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #16 | Type: B | PTS: 16568 | DTS: 16568 | Timestamp: 0.690333s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #17 | Type: B | PTS: 17569 | DTS: 17569 | Timestamp: 0.732042s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #18 | Type: P | PTS: 18570 | DTS: 18570 | Timestamp: 0.77375s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #19 | Type: B | PTS: 19571 | DTS: 19571 | Timestamp: 0.815458s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #20 | Type: B | PTS: 20572 | DTS: 20572 | Timestamp: 0.857167s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #21 | Type: P | PTS: 21573 | DTS: 21573 | Timestamp: 0.898875s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #22 | Type: B | PTS: 22574 | DTS: 22574 | Timestamp: 0.940583s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #23 | Type: P | PTS: 23575 | DTS: 23575 | Timestamp: 0.982292s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b2c4088acb2c896d5d79adbe4b893ac
Frame #24 | Type: I | PTS: 24576 | DTS: 24576 | Timestamp: 1.024s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 3e7b36a03e796e59fd70d449d950fda2
Frame #25 | Type: P | PTS: 25577 | DTS: 25577 | Timestamp: 1.06571s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: a8adabf4ad879aa1f96efec162031348
Frame #26 | Type: P | PTS: 26578 | DTS: 26578 | Timestamp: 1.10742s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: b35c736d14d153e9578e2b2816394364
Frame #27 | Type: P | PTS: 27579 | DTS: 27579 | Timestamp: 1.14912s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: bf0bdba46e640625b5f135bcc73152da
Frame #28 | Type: P | PTS: 28580 | DTS: 28580 | Timestamp: 1.19083s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: f475e1549ee5909b2be26d386c636eef
Frame #29 | Type: P | PTS: 29581 | DTS: 29581 | Timestamp: 1.23254s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 8fec2f4b389b827dbb7a5c0ef6ab4e41
Frame #30 | Type: P | PTS: 30582 | DTS: 30582 | Timestamp: 1.27425s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: dc173ab2a0bad1b2bd6b92f6b32fd72d
Frame #31 | Type: P | PTS: 31583 | DTS: 31583 | Timestamp: 1.31596s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 6a40f2d35095b1275524302b6718d1c6
Frame #32 | Type: P | PTS: 32584 | DTS: 32584 | Timestamp: 1.35767s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: ebf30fddbb83c30622e40dfbc211abae
Frame #33 | Type: P | PTS: 33585 | DTS: 33585 | Timestamp: 1.39938s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 234663701a4cc821bcc5de650ad1f49b
Frame #34 | Type: P | PTS: 34586 | DTS: 34586 | Timestamp: 1.44108s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 629ea2082a1c4173c688db05ae0b9920
Frame #35 | Type: P | PTS: 35587 | DTS: 35587 | Timestamp: 1.48279s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 18e43e13fd898f9349eff63752202875
Frame #36 | Type: P | PTS: 36588 | DTS: 36588 | Timestamp: 1.5245s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 2705f50f0c2b622fac4d0fa13b114df3
Frame #37 | Type: P | PTS: 37589 | DTS: 37589 | Timestamp: 1.56621s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: c679f1aae899cce47f74285bf4ea3f7d
Frame #38 | Type: P | PTS: 38590 | DTS: 38590 | Timestamp: 1.60792s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 02f7bf89b2662b93e1aae9b65dddf14c
Frame #39 | Type: P | PTS: 39591 | DTS: 39591 | Timestamp: 1.64962s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9036699da13f4286ed9af573ecbb5648
Frame #40 | Type: P | PTS: 40592 | DTS: 40592 | Timestamp: 1.69133s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 89f734b2482ab121e5b67cf4ced4fd39
Frame #41 | Type: P | PTS: 41593 | DTS: 41593 | Timestamp: 1.73304s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 323e8534f2b7b50f401c128af66a008f
Frame #42 | Type: B | PTS: 42594 | DTS: 42594 | Timestamp: 1.77475s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 7ed430e5c2065ed256fd77b96eab2295
Frame #43 | Type: P | PTS: 43595 | DTS: 43595 | Timestamp: 1.81646s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 1000ac365c3e410fd72aa7c85036c5b0
Frame #44 | Type: P | PTS: 44596 | DTS: 44596 | Timestamp: 1.85817s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 928f6a34d6cde6a99f7b0e35da4fab4e
[h264 @ 0x5582a34440] cabac decode of qscale diff failed at 116 61
[h264 @ 0x5582a34440] error while decoding MB 116 61, bytestream 1900
[h264 @ 0x5582a34440] concealing 773 DC, 773 AC, 773 MV errors in P frame

=== CORRUPTION DETECTED === 

Packet PTS: 47599
Frame PTS: 46598
Error Flags: 4Frame #45 | Type: P | PTS: 46598 | DTS: 46598 | Timestamp: 1.94158s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 29a5b3c1c8b5652066e76064f495ae2f
[WARNING] Error Concealment Used!!!! 
[Concealment Active] PTS: 46598
Frame #46 | Type: P | PTS: 47599 | DTS: 47599 | Timestamp: 1.98329s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 2cf3f6ac69bd0a49ded5ddec3ad0e954
Frame #47 | Type: P | PTS: 48600 | DTS: 48600 | Timestamp: 2.025s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: dc3044c9713bac5d9af5ace716dd4009
Frame #48 | Type: P | PTS: 49601 | DTS: 49601 | Timestamp: 2.06671s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: bf0affec1c30607b1cd6f67cfa36c699
Frame #49 | Type: P | PTS: 50602 | DTS: 50602 | Timestamp: 2.10842s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 90e93457bf2cffea160b59ecc4f3f558
Frame #50 | Type: P | PTS: 51603 | DTS: 51603 | Timestamp: 2.15013s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: f7703ada7fc62b6bab6fdd72379ea16d
[h264 @ 0x5582a34440] concealing 7989 DC, 7989 AC, 7989 MV errors in B frame

=== CORRUPTION DETECTED === 

Packet PTS: 52604
Frame PTS: 52604
Error Flags: 4Frame #51 | Type: B | PTS: 52604 | DTS: 52604 | Timestamp: 2.19183s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9b6ab2b380be9770bb8bbc0c3efc9102
[WARNING] Error Concealment Used!!!! 
[Concealment Active] PTS: 52604
Frame #52 | Type: P | PTS: 53605 | DTS: 53605 | Timestamp: 2.23354s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: e913fcb2d7f79e165e58f3aa29d5bc06
Frame #53 | Type: P | PTS: 54606 | DTS: 54606 | Timestamp: 2.27525s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 25869274f8f1df997536d9a61705874d
Frame #54 | Type: P | PTS: 55607 | DTS: 55607 | Timestamp: 2.31696s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: c14414666dcb54c03b4ac5f6b35aef53
Frame #55 | Type: B | PTS: 56608 | DTS: 56608 | Timestamp: 2.35867s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 5ac964216756a88f96d8fbedb70a2572
Frame #56 | Type: P | PTS: 57609 | DTS: 57609 | Timestamp: 2.40037s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: c98a6cd4b5ffeac5b8a821c1c30c7dd0
Frame #57 | Type: B | PTS: 58610 | DTS: 58610 | Timestamp: 2.44208s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9986f8649bbdda501a7c730661630756
Frame #58 | Type: P | PTS: 59611 | DTS: 59611 | Timestamp: 2.48379s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 5bbf85674ed6fc09eb09f0d8989839e7
Frame #59 | Type: P | PTS: 60612 | DTS: 60612 | Timestamp: 2.5255s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: b930734ce9905c7706ee3fda113f0ce4
Frame #60 | Type: B | PTS: 61613 | DTS: 61613 | Timestamp: 2.56721s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 9da6c54cb725a349ebebe6880ac2670e
Frame #61 | Type: P | PTS: 62614 | DTS: 62614 | Timestamp: 2.60892s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 90d9257f32ccb3df2617e32548f8fddb
Frame #62 | Type: P | PTS: 63615 | DTS: 63615 | Timestamp: 2.65062s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 984667e0de9b11f1a2b7e9ba5cba894a
Frame #63 | Type: P | PTS: 64616 | DTS: 64616 | Timestamp: 2.69233s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 83a35c77fc6191cf2ce703edcf4b480e
Frame #64 | Type: P | PTS: 65617 | DTS: 65617 | Timestamp: 2.73404s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 7eae1f007869b08213f17458fdbd1209
Frame #65 | Type: P | PTS: 66618 | DTS: 66618 | Timestamp: 2.77575s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: f90be0a165a5e3696a99d030c1ddacff
[Quit]
Frame #66 | Type: P | PTS: 67619 | DTS: 67619 | Timestamp: 2.81746s | Resolution: 1920x1080 | Pixel fmt: yuv420p | Decoded frm MD5: 8c80b6d783b390bcb97f3f674df2626c
root@bleedingedge-rtd1619b:/mnt/2025/06# 
```

