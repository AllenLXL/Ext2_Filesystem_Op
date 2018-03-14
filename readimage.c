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
    int fd = open("/Users/liallen/Desktop/tutex7/emptydisk.img", O_RDWR);

    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    // superblock
    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    // block group descriptor table pointer.
    struct ext2_group_desc *gdt = (struct ext2_group_desc *)(disk + 1024 * 2);

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
        printf(" ");
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
        printf(" ");
    }


    return 0;
}
