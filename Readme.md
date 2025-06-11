# ffmpeg-decode-tool

A C++ utility demonstrating multithreaded video demuxing and seeking using FFmpeg :

Displays frame info along with errors



## main Features

* **Multithreaded Demuxing**: Efficiently reads and decodes multimedia streams using multiple threads.
* **Precise Seeking**: Implements frame-accurate seeking to specific timestamps within video files.
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
ffmpeg_seeker -i corrupted_bitstream.mp4 -d HW -c h264_v4l2m2m  -v info
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

