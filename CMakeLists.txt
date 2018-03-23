cmake_minimum_required(VERSION 3.9)
project(a4)

set(CMAKE_CXX_STANDARD -std=gnu99)

add_executable(a4
        ext2.h
        ext2_checker.c
        ext2_cp.c
        ext2_ln.c
        ext2_mkdir.c
        ext2_restore.c
        ext2_rm.c
        ext2_utils.c
        ext2_utils.h
        )