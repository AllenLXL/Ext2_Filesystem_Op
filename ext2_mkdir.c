//
// Created by LiAllen on 2018-03-19.
//

#include "ext2_utils.h"


unsigned char* disk;


int main(int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <abs path of file sys>", argv[0]);
        exit(1);
    }


    if (argv[1][0]!='/') {
        fprintf(stderr,"not abs path");
        exit(1);
    }
    struct ext2_super_block* sb = (struct ext2_super_block *)(disk + 1024);
    struct ext2_group_desc *gd = (struct ext2_group_desc *)(disk + 2048);
//    struct ext2_group_desc *gd = (struct ext2_group_desc *)(disk + 2048);
//    struct ext2_group_desc *gd = (struct ext2_group_desc *)(disk + 2048);

    if (sb->s_free_blocks_count == 0 || sb->s_free_inodes_count == 0){
        perror("no enough space");
        exit(ENOMEM);
    }
    int in_use;
    int bb_status[sb->s_blocks_count];
    int loop = 0;
    unsigned char *bb_content = disk + (EXT2_BLOCK_SIZE * gd->bg_block_bitmap);
    for (int byte = 0; byte < (sb->s_blocks_count/8); byte++){
        for (int bit = 0; bit < 8; bit++){
            in_use = bb_content[byte] & (1 << bit);
            if (in_use){
                bb_status[loop] = 1;
            } else{
                bb_status[loop] = 0;
            }
            loop++;
        }
    }

    int ib_status[sb->s_inodes_count];
    loop = 0;
    unsigned char *ib_content = disk + (EXT2_BLOCK_SIZE * gd->bg_inode_bitmap);
    for (int byte = 0; byte < (sb->s_inodes_count/8); byte++){
        for (int bit = 0; bit < 8; bit++){
            in_use = bb_content[byte] & (1 << bit);
            if (in_use){
                ib_status[loop] = 1;
            } else{
                ib_status[loop] = 0;
            }
            loop++;
        }
    }
    // find free block and free inode now
    struct ext2_inode * new_dir;
    for (int i = 0; i < sb->s_blocks_count; i++){
        if (bb_status[i]==0){
            int by = i / 8;
            int remainder = i % 8;
            bb_content[by] |= 1 << (8-remainder);
            new_dir = (struct ext2_inode *)(disk + gd->bg_inode_table * 1024 + i * sb->s_inode_size);
            break;
        }
    }


    for (int j = 0; j < sb->s_blocks_count; j++){
        if (ib_status[j]==0){
            int by = j / 8;
            int remainder = j % 8;
            ib_content[by] |= 1 << (8-remainder);
            new_dir->i_block[0] = (unsigned int) (j + 1);
            break;
        }
    }

    new_dir->i_mode = EXT2_S_IFDIR; //set inode type
//    new_dir->i_uid = 0; //as directed TODO wtf is this?
//    new_dir->i_gid = 0; //
    new_dir->i_size = EXT2_BLOCK_SIZE;  // 1024 == dir size
    new_dir->i_links_count = 2; //Newly created direcotry  TODO why?
//    new_dir->osd1 = 0;

    new_dir->i_blocks = 2;




}