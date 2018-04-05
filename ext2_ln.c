/*
 * Implementing ln liked operations
 * Created by LiAllen on 2018-03-19.
 */

#include "ext2_utils.h"

int main(int argc, char **argv) {
    // trivial stuff
    if (argc != 4 && argc !=5) {
        fprintf(stderr, "Usage: %s <image file name> (-s) <abs path_1 of ext2> <abs path_2 of ext2>\n", argv[0]);
        exit(1);
    }
    // if symbolic
    int symbolic = 0;
    // make sure flag is valid
    if (argc==5 && strncmp("-s", argv[2],2)!=0){
        fprintf(stderr, "invalid arguments\n");
        exit(1);
    }
    if (argc == 5 && strncmp("-s", argv[2],2)==0){
        symbolic =1;
    }
    if (argc == 5&& strncmp("-s", argv[2],2)==0){
        validate_path(argv[3]);
        validate_path(argv[4]);
    } else{
        validate_path(argv[2]);
        validate_path(argv[3]);
    }

    // init all global vars
    init_ptrs(argv[1]);

    first_front=NULL;
    second_front=NULL;

    // init link list according to type of links
    if (argc ==5){
        construct_ll(argv[3], &first_front);
        construct_ll(argv[4], &second_front);
    } else{
        construct_ll(argv[2], &first_front);
        construct_ll(argv[3], &second_front);
    }

    // target name
    char* name_1 = get_last_name(first_front);
    char* name_2 = get_last_name(second_front);

    // get to target parent dir ent
    struct ext2_dir_entry* parent_dir_fir = get_parent_dir_block(first_front);
    struct ext2_dir_entry* parent_dir_sec = get_parent_dir_block(second_front);

    // error handling
    if (check_type(parent_dir_sec, name_2)!=0){
        printf("same name DIR/REG exist already\n");
        exit(EEXIST);
    }

    if (check_type(parent_dir_fir, name_1)==2 && (!symbolic)){
        printf("cannot hard link directory\n");
        return EISDIR;
    }

    // if symbolic, new inode and block needed
    if (symbolic){
        // allocate new inode
        int free_inode_idx = find_free_inode() + 1;
        set_bitmap(0, free_inode_idx, 1);
        sb->s_free_inodes_count--;
        gdt->bg_free_inodes_count--;

        struct ext2_inode* new_inode = &inode_table[free_inode_idx - 1];
        init_inode(new_inode);
        // init attr
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
        // transfer data to block
        memcpy(disk + EXT2_BLOCK_SIZE*free_block_idx, argv[3], path_len);
        struct ext2_dir_entry* new_add = add_parent_block(parent_dir_sec, name_2, EXT2_FT_SYMLINK);
        new_add->inode = (unsigned int) free_inode_idx;
    } else{
        // hardlink only requires add an entry
        struct ext2_dir_entry* new_add = add_parent_block(parent_dir_sec, name_2, EXT2_FT_REG_FILE);
        struct ext2_dir_entry* link = get_dir_ent(parent_dir_fir, name_1);
        // update newly added entry's inode
        new_add->inode = link->inode;
        inode_table[new_add->inode-1].i_links_count++;
    }

    // free all malloc data
    free_ll(first_front);
    free_ll(second_front);
    free(name_1);
    free(name_2);
    return 0;
}