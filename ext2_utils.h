#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <memory.h>
#include <time.h>
#include "ext2.h"

#ifndef A4_EXT2_UTILS_H
#define A4_EXT2_UTILS_H

unsigned char *disk;

// super block
struct ext2_super_block *sb;

// group description table
struct ext2_group_desc *gdt;

// block bitmap
unsigned char *block_bm;

// inode bitmap
unsigned char *inode_bm;

// inode table
struct ext2_inode *inode_table;

// linked list for directory.
typedef struct linked_list{
    struct linked_list* next;
    char* name;
    int name_len;
} ll;

// linked list for directory entry
typedef struct dir_ent_linklst{
    struct dir_ent_linklst* next;
    struct ext2_dir_entry* dir_ent;
    int new_block;
} dir_ll;

// first element in dir_ll
dir_ll* dir_ll_head;

// first element in ll
ll* first_front;

// also first element in ll
ll* second_front;

//check if path is absolute
void validate_path(char* path);

// initialize the disk
void init_ptrs(char* img_file);

// find the next free inode
int find_free_inode();

// find the next free block
int find_free_block();

// set block/inode bitmap
void set_bitmap(int bm_idx ,int idx, int mode);

// construct a ll with a absolute path
void construct_ll(char* path, ll** link_list);

// initialize a inode
void init_inode(struct ext2_inode* new_inode);

// return the parent directory block
struct ext2_dir_entry* get_parent_dir_block(ll* link_list_head);

// length of a linked list
int get_ll_length(ll* head);

// name of last file in linked list
char* get_last_name(ll* ll_head);

// add a new directory entry to it's parent with name and type
struct ext2_dir_entry* add_parent_block(struct ext2_dir_entry* dir_entry, char* name, int type);

// Given parent directory entry and name, we determine its type.
int check_type(struct ext2_dir_entry* first_dir_ent , char* name);

// find a file/directory/link in a folder and return it's directory entry.
struct ext2_dir_entry* get_dir_ent(struct ext2_dir_entry* par_fir_ent, char* name);

// return a new dir linked list which start with dir_entry
void constrcut_dir_ll(struct ext2_dir_entry* dir_entry);

// compare if two entries has a same type
int compare(int entry_type, int inode_type);

// check if a file in a directory and if errors happen, correct it,
int check_files_in_dir(int inode_idx);

// check block bitmap match real blocks
int check_blocks(int inode_idx);

// get the block/inode bitmap's status
int get_bitmap(int bm_idx, int idx);

// recursively remove a directory
void release_all(int inode_idx);

// return a new dir linked list which start with dir_entry
dir_ll*constrcut_dir_ll_spe(struct ext2_dir_entry* dir_entry, dir_ll* head);

// return dir ent's minimal possible record length.
int get_rec_len(struct ext2_dir_entry* dir_ent);

// free linked list and free dir linked list
void free_ll(ll* head);
void free_dir_ll(dir_ll* head);

#endif //A4_EXT2_UTILS_H
