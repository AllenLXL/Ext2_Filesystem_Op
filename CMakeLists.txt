cmake_minimum_required(VERSION 3.9)
project(a4)

set(CMAKE_CXX_STANDARD -std=gnu99)

add_executable(ext2_cp
        ext2.h
        ext2_cp.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(ext2_mkdir
        ext2.h
        ext2_mkdir.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(readimage
        ext2.h
        readimage.c
        )