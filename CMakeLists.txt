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
add_executable(ext2_ln
        ext2.h
        ext2_ln.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(ext2_rm
        ext2.h
        ext2_rm.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(ext2_rm_bonus
        ext2.h
        ext2_rm_bonus.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(ext2_restore
        ext2.h
        ext2_restore.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(ext2_checker
        ext2.h
        ext2_checker.c
        ext2_utils.c
        ext2_utils.h
        )

add_executable(readimage
        ext2.h
        readimage.c
        )