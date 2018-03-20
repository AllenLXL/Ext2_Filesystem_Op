//
// Created by LiAllen on 2018-03-19.
//
#include "ext2.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

unsigned char* disk;


int main(int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <abs path of file sys>", argv[0]);
        exit(1);
    }
    int fd = open(argv[1], O_RDWR);
    if (fd < 0){
        perror("Error opening file.");
        exit(ENOENT);
    }

    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    struct ext2_super_block* sb = (struct ext2_super_block *)(disk + 1024);




}