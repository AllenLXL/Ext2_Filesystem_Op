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

struct ext2_super_block *sb;
struct ext2_group_desc *gdt;

unsigned char *block_bm;
unsigned char *inode_bm;

struct ext2_inode *inode_table;

void print_bm();
void print_dir_block(struct ext2_dir_entry* first_row);
void print_inode();


typedef struct linked_list{
    struct linked_list* next;
    char* name;
    int name_len;
} ll;

typedef struct dir_ent_linklst{
    struct dir_ent_linklst* next;
    struct ext2_dir_entry* dir_ent;
} dir_ll;
dir_ll* dir_ll_head;

ll* first_front;

ll* second_front;


void validate_path(char* path);

void init_ptrs(char* img_file);
int find_free_inode();
int find_free_block();
void set_bitmap(int bm_idx ,int idx, int mode);
void construct_ll(char* path, ll** link_list);
void init_inode(struct ext2_inode* new_inode);


char* convert_path(char* path);
char* get_new_dir_name(char* path);


unsigned int modify_parent_block();

struct ext2_dir_entry* get_parent_dir_block(ll* link_list_head);

void check_existence(struct ext2_dir_entry* first_dir_ent, char* name);

int get_ll_length(ll* head);
char* get_last_name(ll* ll_head);

char* get_sec_last_name(ll* ll_head, int ll_lenth);

struct ext2_dir_entry* add_parent_block(struct ext2_dir_entry* dir_entry, char* name, int type);

int check_type(struct ext2_dir_entry* first_dir_ent , char* name);

int get_inode_idx(struct ext2_dir_entry* first_dir_ent, char* name, int type);
struct ext2_dir_entry* get_dir_ent(struct ext2_dir_entry* par_fir_ent, char* name);

void constrcut_dir_ll(struct ext2_dir_entry* dir_entry);
int compare(int entry_type, int inode_type);
int check_files_in_dir(int inode_idx);
#endif //A4_EXT2_UTILS_H


