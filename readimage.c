#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ext2.h"

unsigned char *disk;


int main(int argc, char **argv) {

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <image file name>\n", argv[0]);
        exit(1);
    }

    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    // superblock
    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    // block group descriptor table pointer.
    struct ext2_group_desc *gdt = (struct ext2_group_desc *)(disk + 1024 * 2);

    printf("s_log_block_size: %d\n", sb->s_log_block_size);
    printf("Inodes: %d\n", sb->s_inodes_count);
    printf("Blocks: %d\n", sb->s_blocks_count);
    printf("Block group:\n");
    printf("    block bitmap: %d\n", gdt->bg_block_bitmap);
    printf("    inode bitmap: %d\n", gdt->bg_inode_bitmap);
    printf("    inode table: %d\n", gdt->bg_inode_table);
    printf("    free blocks: %d\n", gdt->bg_free_blocks_count);
    printf("    free inodes: %d\n", gdt->bg_free_inodes_count);
    printf("    used_dir: %d\n", gdt->bg_used_dirs_count);

    int in_use;
    unsigned char *bb_content = disk + (EXT2_BLOCK_SIZE * gdt->bg_block_bitmap);
    printf("Block bitmap: ");
    for (int byte = 0; byte < 16; byte++){
        for (int bit = 0; bit < 8; bit++){
            in_use = bb_content[byte] & (1 << bit);
            if (in_use){
                printf("%d", 1);
            } else{
                printf("%d", in_use);
            }
        }
        if (byte!=16-1){
            printf(" ");
        }
    }
    printf("\n");
    unsigned char *ib_content = disk + (EXT2_BLOCK_SIZE * gdt->bg_inode_bitmap);
    printf("Inode bitmap: ");

    for (int byte = 0; byte < 32 / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            in_use = ib_content[byte] & (1 << bit);
            if (in_use){
                printf("%d", 1);
            } else{
                printf("%d", in_use);
            }
        }
        if (byte!=4-1){
            printf(" ");
        }
    }
    printf("\n\n");

    printf("Inodes:\n");
    struct ext2_inode *inode = (struct ext2_inode *)(disk + EXT2_BLOCK_SIZE * gdt->bg_inode_table);

    for (int i = EXT2_ROOT_INO - 1; i < sb->s_inodes_count; i++) {
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO) {

            if (inode[i].i_size != 0) {
                if (inode[i].i_mode & EXT2_S_IFREG) { // regular
                    printf("[%d] type: f size: %d links: %d blocks: %d\n", i + 1, inode[i].i_size,
                           inode[i].i_links_count, inode[i].i_blocks);
                    printf("[%d] Blocks: ", i + 1);
                    if (inode[i].i_blocks / 2 < 12){
                        for (int j = 0; j < (inode[i].i_blocks / 2); j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        printf("\n");
                    } else {
                        for (int j = 0; j < 13; j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        int* location = (int *)(disk + EXT2_BLOCK_SIZE*inode[i].i_block[12]);
                        for (int k = 0; k < (inode[i].i_blocks / 2)-12-1; k++) {
                            if (k!=(inode[i].i_blocks / 2)-12-1-1){
                                printf("%d ", location[k]);
                            } else{
                                printf("%d", location[k]);
                            }
                        }
                        printf("\n");
                    }
                }
                if (inode[i].i_mode & EXT2_S_IFDIR) { // directory
                    printf("[%d] type: d size: %d links: %d blocks: %d\n", i + 1, inode[i].i_size,
                           inode[i].i_links_count, inode[i].i_blocks);
                    printf("[%d] Blocks: ", i + 1);
                    if (inode[i].i_blocks / 2 < 12){
                        for (int j = 0; j < (inode[i].i_blocks / 2); j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        printf("\n");
                    } else {
                        for (int j = 0; j < 13; j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        int* location = (int *)(disk + EXT2_BLOCK_SIZE*inode[i].i_block[12]);
                        for (int k = 0; k < (inode[i].i_blocks / 2)-12-1; k++) {
                            if (k!=(inode[i].i_blocks / 2)-12-1-1){
                                printf("%d ", location[k]);
                            } else{
                                printf("%d", location[k]);
                            }
                        }
                        printf("\n");
                    }
                }
            }
        }
    }
    printf("\n");
    printf("Directory Blocks:\n");
    struct ext2_inode *all_inode = (struct ext2_inode *)(disk + EXT2_BLOCK_SIZE * gdt->bg_inode_table);
    for (int i = EXT2_ROOT_INO - 1; i < sb->s_inodes_count; i++) {
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO) {
            if (all_inode[i].i_size != 0) {
                if (all_inode[i].i_mode & EXT2_S_IFDIR){
                    for (int j = 0; j < (all_inode[i].i_blocks/2); j++) {
                        printf("   DIR BLOCK NUM: %d (for inode %d)\n", all_inode[i].i_block[j], i + 1);
                        struct ext2_dir_entry * dir_entry = (struct ext2_dir_entry *)(disk + all_inode[i].i_block[j]*EXT2_BLOCK_SIZE);
                        int k = 0;
                        while (k < EXT2_BLOCK_SIZE) {
                            if (EXT2_FT_REG_FILE == dir_entry->file_type) {
                                printf("Inode: %d rec_len: %d name_len: %d type= f name=%.*s\n",
                                       dir_entry->inode, dir_entry->rec_len, dir_entry->name_len,dir_entry->name_len,dir_entry->name);
                            }
                            else if (EXT2_FT_DIR == dir_entry->file_type) {
                                printf("Inode: %d rec_len: %d name_len: %d type= d name=%.*s\n",
                                       dir_entry->inode, dir_entry->rec_len, dir_entry->name_len,dir_entry->name_len,dir_entry->name);
                            }
                            k += dir_entry->rec_len;
                            dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
                        }
                    }
                }
            }
        }
    }
    return 0;
}