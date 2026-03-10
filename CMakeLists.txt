cmake_minimum_required(VERSION 3.16)

project(midi2ogg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

# Find libraries
pkg_check_modules(ADLMIDI REQUIRED adlmidi)
pkg_check_modules(VORBIS REQUIRED vorbis vorbisenc)
pkg_check_modules(OGG REQUIRED ogg)

add_executable(midi2ogg
    src/adlmidi2ogg.cpp
)

target_include_directories(midi2ogg PRIVATE
    ${ADLMIDI_INCLUDE_DIRS}
    ${VORBIS_INCLUDE_DIRS}
    ${OGG_INCLUDE_DIRS}
)

target_link_libraries(midi2ogg
    ${ADLMIDI_LIBRARIES}
    ${VORBIS_LIBRARIES}
    ${OGG_LIBRARIES}
)

target_compile_options(midi2ogg PRIVATE
    ${ADLMIDI_CFLAGS_OTHER}
    ${VORBIS_CFLAGS_OTHER}
    ${OGG_CFLAGS_OTHER}
)
