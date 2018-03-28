//
// Created by LiAllen on 2018-03-19.
//

#include "ext2_utils.h"


int main(int argc, char **argv) {
    if (argc != 4 && argc !=5) {
        fprintf(stderr, "Usage: %s <image file name> <abs path_1 of ext2> <abs path_2 of ext2> (-s)\n", argv[0]);
        exit(1);
    }
    int symbolic = 0;
    if (argc == 5){
        symbolic =1;
    }
    validate_path(argv[2]);
    validate_path(argv[3]);

    init_ptrs(argv[1]);

    first_front=NULL;
    second_front=NULL;

    construct_ll(argv[2], &first_front);
    construct_ll(argv[3], &second_front);

    char* name_1 = get_last_name(first_front);
    char* name_2 = get_last_name(second_front);

    struct ext2_dir_entry* parent_dir_fir = get_parent_dir_block(first_front);
    struct ext2_dir_entry* parent_dir_sec = get_parent_dir_block(second_front);

    struct ext2_dir_entry* fir_dir = get_dir_ent(parent_dir_fir, name_1);
    struct ext2_dir_entry* sec_dir = get_dir_ent(parent_dir_sec, name_2);

    if (check_type(parent_dir_fir, name_2)==1){
        printf("exist already\n");
        exit(EEXIST);
    }

    if (check_type(parent_dir_fir, name_1)==2){
        printf("cannot link directory\n");
        return EISDIR;
    }

    if (symbolic){
        int free_inode_idx = find_free_inode() + 1;
        set_bitmap(0, free_inode_idx, 1);
        sb->s_free_inodes_count--;
        gdt->bg_free_inodes_count--;

        struct ext2_inode* new_inode = &inode_table[free_inode_idx - 1];
        init_inode(new_inode);

        new_inode->i_mode=EXT2_S_IFLNK;
        int path_len = (int) strlen(argv[3]);
        new_inode->i_size= (unsigned int) path_len;
        new_inode->i_links_count = 1;
        int free_block_idx = find_free_block() + 1;
        set_bitmap(1, free_block_idx, 1);
        memcpy(disk + EXT2_BLOCK_SIZE*free_block_idx, argv[3], path_len);

        add_parent_block(parent_dir_fir, name_2, EXT2_FT_SYMLINK);
    } else{
        add_parent_block(parent_dir_fir, name_2, EXT2_FT_REG_FILE);
    }



}