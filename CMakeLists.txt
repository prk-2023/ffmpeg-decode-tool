cmake_minimum_required(VERSION 3.10)
project(ffmpeg_seeker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(SPDLOG_FMT_EXTERNAL)
add_compile_options(-Wall)

# find ffmpeg using pkg-config ( this also works with pc)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil)

# include link paths
include_directories(${FFMPEG_INCLUDE_DIRS})
link_directories(${FFMPEG_LIBRARY_DIRS})
add_definitions(${FFMPEG_CFLAGS_OTHER})

# fmt (prefer modern target if available)
find_package(fmt REQUIRED)

add_executable(ffmpeg_seeker demux_seek_threaded.cpp)

target_link_libraries(ffmpeg_seeker
    ${FFMPEG_LIBRARIES}
    fmt::fmt
    pthread
)

