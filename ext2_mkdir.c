//
// Created by LiAllen on 2018-03-19.
//

#include "ext2_utils.h"


int main(int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <abs path of ext2 file sys>", argv[0]);
        exit(1);
    }
    init_ptrs(argv[1]);
    char* good_path = convert_path(argv[2]);
    construct_ll(good_path, first_front);

    // this idx starts from 1
    int free_inode_idx = find_free_inode() + 1;
    int free_block_idx = find_free_block() + 1;

    struct ext2_inode* new_inode = &inode_table[free_inode_idx - 1];

    new_inode->i_mode = EXT2_S_IFDIR;
    new_inode->i_size = EXT2_BLOCK_SIZE;
    new_inode->i_links_count = 2;
    new_inode->i_blocks = 2;
    init_inode(new_inode);

    new_inode->i_block[0] = (unsigned int) free_block_idx - 1;
    int i;
    for(i = 1; i < 15; i++) {
        new_inode->i_block[i] = 0;
    }

    unsigned int parent_idx = modify_parent_block() - 1;

    //set first dir_entry to itself
    struct ext2_dir_entry *self_dir = (void*)disk + ((free_block_idx-1) * EXT2_BLOCK_SIZE);
    self_dir->inode = (unsigned int) free_inode_idx;
    self_dir->rec_len = 12;
    self_dir->name_len = 1;
    self_dir->file_type = EXT2_FT_DIR;
    strncpy(self_dir->name, ".", 1);

    //set another dir_entry to = parent
    struct ext2_dir_entry* parent_dir = (void*)self_dir + self_dir->rec_len;
    parent_dir->inode = ((struct ext2_dir_entry *)(disk + (inode_table[parent_idx].i_block[0] * EXT2_BLOCK_SIZE)))->inode;
    parent_dir->rec_len = EXT2_BLOCK_SIZE - 12;
    parent_dir->name_len = 2;
    parent_dir->file_type = EXT2_FT_DIR;
    strncpy(parent_dir->name, "..", 2);
    inode_table[parent_idx].i_links_count++;

    print_dir_block(self_dir);


    // switch bitmap off
    set_bitmap(0, free_inode_idx, 1);
    set_bitmap(1, free_block_idx, 1);

    // modify super block
    sb->s_free_inodes_count--;
    sb->s_free_blocks_count--;

    // modify group descriptor table
    gdt->bg_free_inodes_count--;
    gdt->bg_free_blocks_count--;
    gdt->bg_used_dirs_count++;

}