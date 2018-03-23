//
// Created by LiAllen on 2018-03-21.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <memory.h>
#include "ext2.h"

#ifndef A4_EXT2_UTILS_H
#define A4_EXT2_UTILS_H

unsigned char *disk;

struct ext2_super_block *sb;
struct ext2_group_desc *gdt;

unsigned char *block_bm;
unsigned char *inode_bm;

struct ext2_inode *inode_table;

int init_ptrs(char* img_file);

int find_free_inode();

int find_free_block();

void set_bitmap(int bm_idx ,int idx, int mode);

char* convert_path(char* path);
char* get_new_dir_name(char* path);

void construct_ll(char* path);
unsigned int modify_parent_block();

typedef struct linked_list{
    struct linked_list* next;
    char* name;
    int name_len;
} ll;

ll* front;
int ll_length;

#endif //A4_EXT2_UTILS_H


