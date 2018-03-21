//
// Created by LiAllen on 2018-03-19.
//

#include "ext2_utils.h"


/*
 * This function is for initialize all necessary pointer for every operations.
 * On success return image file descriptor, o.w. exit.
 */
int init_ptrs(char* img_file){
    int fd = open(img_file, O_RDWR);

    if (fd < 0){
        perror("Error opening file.");
        exit(ENOENT);
    }

    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    sb = (struct ext2_super_block *)(disk + EXT2_BLOCK_SIZE);
    gdt = (struct ext2_group_desc *)(disk + EXT2_BLOCK_SIZE * 2);
    block_bm = disk + (EXT2_BLOCK_SIZE * gdt->bg_block_bitmap);
    inode_bm = disk + (EXT2_BLOCK_SIZE * gdt->bg_inode_bitmap);
    inode_table = (struct ext2_inode *)(disk + EXT2_BLOCK_SIZE * gdt->bg_inode_table);

    return fd;
}
/*
 * This function is used for finding a free inode in inode bitmap.
 * If there is no free inode now, return -1 indicating error.
 */
int find_free_inode(){
    int idx = 0;
    for (int byte = 0; byte < sb->s_inodes_count / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (inode_bm[byte] & (1 << bit)){
                idx++;
            } else{
                return idx;
            }
        }
    }
    return -1;
}

/*
 * This function is used for finding a free block in block bitmap.
 * If there is no free block now, return -1 indicating error.
 */
int find_free_block(){
    int idx = 0;
    for (int byte = 0; byte < sb->s_blocks_count / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (block_bm[byte] & (1 << bit)){
                idx++;
            } else{
                return idx;
            }
        }
    }
    return -1;
}

/*
 * This function is used to set bits of inode/block bitmap on or off.
 * bm_idx can be 0 or 1 indicates inode bitmap or block bitmap
 * idx indicates bit index in bitmap. if mode is 0, turn idx on, o.w. off.
 */
void set_bitmap(int bm_idx ,int idx, int mode){
    int num_bit;
    if (!bm_idx){
        num_bit = sb->s_blocks_count;
    } else{
        num_bit = sb->s_inodes_count;
    }
    int cur_idx = 0;

    for (int byte = 0; byte < num_bit / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (cur_idx == idx){
                if (mode){
                    block_bm[byte] = (unsigned char) (block_bm[byte] | (1 << bit));
                } else{
                    int temp = 0xff;
                    temp = temp ^ (1 << bit);
                    block_bm[byte] = (unsigned char) (block_bm[byte] & temp);
                }
            }
        }
    }
}
