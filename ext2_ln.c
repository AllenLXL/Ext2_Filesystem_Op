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
    if (argc == 5){
        validate_path(argv[3]);
        validate_path(argv[4]);
    } else{
        validate_path(argv[2]);
        validate_path(argv[3]);
    }

    init_ptrs(argv[1]);

    first_front=NULL;
    second_front=NULL;

    if (argc ==5){
        construct_ll(argv[3], &first_front);
        construct_ll(argv[4], &second_front);
    } else{
        construct_ll(argv[2], &first_front);
        construct_ll(argv[3], &second_front);
    }


    char* name_1 = get_last_name(first_front);
    char* name_2 = get_last_name(second_front);

    struct ext2_dir_entry* parent_dir_fir = get_parent_dir_block(first_front);
    struct ext2_dir_entry* parent_dir_sec = get_parent_dir_block(second_front);


    if (check_type(parent_dir_sec, name_2)!=0){
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
        new_inode->i_blocks+=2;
        new_inode->i_block[0]= (unsigned int) free_block_idx;
        set_bitmap(1, free_block_idx, 1);
        sb->s_free_blocks_count--;
        gdt->bg_free_blocks_count--;
        memcpy(disk + EXT2_BLOCK_SIZE*free_block_idx, argv[3], path_len);
//        print_dir_block(parent_dir_sec);
        struct ext2_dir_entry* new_add = add_parent_block(parent_dir_sec, name_2, EXT2_FT_SYMLINK);
//        print_dir_block(new_add);
        new_add->inode = (unsigned int) free_inode_idx;
    } else{
        struct ext2_dir_entry* new_add = add_parent_block(parent_dir_sec, name_2, EXT2_FT_REG_FILE);
        struct ext2_dir_entry* link = get_dir_ent(parent_dir_fir, name_1);
        new_add->inode = link->inode;
        inode_table[new_add->inode-1].i_links_count++;
    }
    return 0;
}