//
// Created by LiAllen on 2018-03-19.
//

#include "ext2_utils.h"


int main(int argc, char **argv) {
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <image file name> <local file path> <abs path of ext2 file sys>", argv[0]);
        exit(1);
    }

    validate_path(argv[3]);

    // validate local path
    if (access(argv[2], F_OK)!=0){
        fprintf(stderr, "invalid local file path");
        exit(ENOENT);
    }

    init_ptrs(argv[1]);

    first_front=NULL;
    second_front=NULL;

    construct_ll(argv[2], &second_front);

    construct_ll(argv[3], &first_front);



    FILE *source_fd = fopen(argv[2], "rb");
    // get source file size
    fseek(source_fd, 0L, SEEK_END);
    long source_size = ftell(source_fd);
    // ptr points back
    fseek(source_fd, 0, SEEK_SET);

    int block_need = (int) ((source_size + EXT2_BLOCK_SIZE - 1) / EXT2_BLOCK_SIZE); // taking ceil
    if (block_need>12){
        block_need++;   //one more for indirection block
    }

    if (sb->s_free_blocks_count < block_need) {
        fprintf(stderr, "No block available");
        exit(1);
    }

    if (sb->s_free_inodes_count < 1){
        fprintf(stderr, "No inode available");
        exit(1);
    }

    // this idx starts from 1
    int free_inode_idx = find_free_inode() + 1;
    set_bitmap(0, free_inode_idx, 1);
    sb->s_free_blocks_count--;
    gdt->bg_free_inodes_count--;

    struct ext2_inode* new_inode = &inode_table[free_inode_idx - 1];
    init_inode(new_inode);
    new_inode->i_mode=EXT2_S_IFREG;
    new_inode->i_size= (unsigned int) source_size;
    new_inode->i_links_count = 1;
    new_inode->i_blocks = (unsigned int) (2 * block_need);

    int free_block_idx;

    for (int i=0; i<12&&i<block_need; i++) {
        // this idx starts from 1
        free_block_idx = find_free_block() + 1;
        new_inode->i_block[i] = (unsigned int) free_block_idx;
        memcpy(disk + EXT2_BLOCK_SIZE * (free_block_idx - 1), source_fd, source_size);
        source_size -= EXT2_BLOCK_SIZE;
        fseek(source_fd, EXT2_BLOCK_SIZE, SEEK_CUR);

        set_bitmap(1, free_block_idx, 1);
        sb->s_free_blocks_count--;
        gdt->bg_free_blocks_count--;
        new_inode->i_blocks += 2;
    }

    if (block_need>12){
        free_block_idx = find_free_block() + 1;
        new_inode->i_block[12]=(unsigned int) free_block_idx;
        unsigned *indirection = (unsigned int *)(disk + ((new_inode->i_block[12]-1) * EXT2_BLOCK_SIZE));

        for (int j = 0; j < block_need-12-1; j++) {
            free_block_idx = find_free_block() + 1;
            indirection[j] = (unsigned int) free_block_idx;
            memcpy(disk + EXT2_BLOCK_SIZE * (free_block_idx-1), source_fd, source_size);
            source_size-=EXT2_BLOCK_SIZE;
            fseek(source_fd, EXT2_BLOCK_SIZE, SEEK_CUR);

            set_bitmap(1, free_block_idx, 1);
            sb->s_free_blocks_count--;
            gdt->bg_free_blocks_count--;
            new_inode->i_blocks+=2;
        }
    }
    struct ext2_dir_entry* dir_entry = get_parent_dir_block(first_front);
    char* target_name = get_last_name(second_front);
    check_existence(dir_entry, target_name, 1);
    add_parent_block(dir_entry, target_name, EXT2_FT_REG_FILE);

}