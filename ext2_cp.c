//
// Created by LiAllen on 2018-03-19.
//

#include "ext2_utils.h"

int main(int argc, char **argv) {
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <image file name> <local file path> <abs path of ext2 file sys>", argv[0]);
        exit(1);
    }
    init_ptrs(argv[1]);

    char* good_path = convert_path(argv[2]);
    construct_ll(good_path, second_front);

    char* another_good_path = convert_path(argv[3]);
    construct_ll(good_path, first_front);

    struct ext2_dir_entry* first_row = get_parent_dir_block(first_front);

    // validate local path
    if (access(argv[2], F_OK)!=0){
        fprintf(stderr, "invalid local file path");
        exit(ENOENT);
    }

    FILE *source_fd = fopen(argv[2], "rb");
    // get source file size
    fseek(source_fd, 0L, SEEK_END);
    long source_size = ftell(source_fd);

    ll* loop_node = second_front;
    while (loop_node->next!=NULL){
        loop_node = loop_node->next;
    }

    check_existence(first_row, loop_node->name, 1);

    int block_need = (int) ((source_size + EXT2_BLOCK_SIZE - 1) / EXT2_BLOCK_SIZE); // taking ceil

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

    if (block_need > 12){
        for (int i=0; i<block_need; i++){
            // this idx starts from 1
            int free_block_idx = find_free_block() + 1;
            set_bitmap(1, free_block_idx, 1);
            sb->s_free_blocks_count--;
            gdt->bg_free_blocks_count--;
        }
    } else{

    }




}