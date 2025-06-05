cmake_minimum_required(VERSION 3.10)
project(ffmpeg_seeker)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add compile options
add_compile_definitions(SPDLOG_FMT_EXTERNAL)
add_compile_options(-Wall)

# Include directories
include_directories(/usr/include/ffmpeg)

# executable
add_executable(ffmpeg_seeker demux_seek_threaded.cpp)

# Link libs
target_link_libraries(ffmpeg_seeker
    avformat
    avcodec
    avutil
    pthread
    fmt
)

