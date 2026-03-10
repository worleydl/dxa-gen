cmake_minimum_required(VERSION 3.16)

project(midi2ogg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

# Find libraries
pkg_check_modules(VORBIS REQUIRED vorbis vorbisenc)
pkg_check_modules(OGG REQUIRED ogg)

# We want the highest quality possible for offline rendering
set(WITH_HQ_RESAMPLER ON CACHE BOOL "" FORCE)
add_subdirectory(ext/libadlmidi)

add_executable(midi2ogg
    src/adlmidi2ogg.cpp
)

target_include_directories(midi2ogg PRIVATE
    ext/libadlmidi/include
    ${VORBIS_INCLUDE_DIRS}
    ${OGG_INCLUDE_DIRS}
)

target_link_libraries(midi2ogg
    ADLMIDI_static
    zita-resampler
    ${VORBIS_LIBRARIES}
    ${OGG_LIBRARIES}
)

target_compile_options(midi2ogg PRIVATE
    ${VORBIS_CFLAGS_OTHER}
    ${OGG_CFLAGS_OTHER}
)
