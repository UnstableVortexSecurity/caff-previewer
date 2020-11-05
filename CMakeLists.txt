cmake_minimum_required(VERSION 3.13)
project(caff_previewer C)

set(CMAKE_C_STANDARD 11)

add_executable(caff_previewer src/main.c src/magic_memory.c src/magic_memory.h src/ciff_tools.h src/ciff_tools.c src/libtarga.c src/libtarga.h src/pixeldata_utils.c src/pixeldata_utils.h src/utils.c src/utils.h)