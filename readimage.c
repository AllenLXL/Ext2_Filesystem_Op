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
    int fd = open(argv[1], O_RDWR);

    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    struct ext2_group_desc *sc = (struct ext2_group_desc *)(disk + 1024 + 1024);
    printf("Inodes: %d\n", sb->s_inodes_count);
    printf("Blocks: %d\n", sb->s_blocks_count);
    printf("Block group: \n");
    printf("    block bitmap: %d\n", sc->bg_block_bitmap);
    printf("    inode bitmap: %d\n", sc->bg_inode_bitmap);
    printf("    inode table: %d\n", sc->bg_inode_table);
    printf("    free blocks: %d\n", sc->bg_free_blocks_count);
    printf("    free inodes: %d\n", sc->bg_free_inodes_count);
    printf("    used_dirs: %d\n", sc->bg_used_dirs_count);

    int in_use;
    printf("Block bitmap:");
    unsigned char *block_bitmap = disk + 1024*sc->bg_block_bitmap;
    for(int i = 0; i < sb->s_blocks_count/8; i++){
        printf(" ");
        for (int j = 0; j<8; j++){
            in_use = block_bitmap[i] & (1 << j);
            printf("%d", in_use && 1);
        }
    }
    printf("\n");

    unsigned char *inode_bitmap = disk + 1024*sc->bg_inode_bitmap;
    printf("Inode bitmap:");
    for(int i = 0; i < sb->s_inodes_count/8; i++){
        printf(" ");
        for (int j = 0; j<8; j++){
            in_use = inode_bitmap[i] & (1 << j);
            printf("%d", in_use && 1);
        }
    }
    printf("\n");

    struct ext2_inode *inode = (struct ext2_inode *)(disk + 1024*sc->bg_inode_table);
    int ind = 0;
    char dtype;

    printf("Inodes:\n");
    for(int i = 0; i < sb->s_inodes_count/8; i++) {
        for(int j = 0; j < 8; j++) {
            if(inode_bitmap[i] & (1 << j)) {
                if (inode[ind].i_mode & EXT2_S_IFDIR) {
                    dtype = 'd';
                } else if(inode[ind].i_mode & EXT2_S_IFREG) {
                    dtype = 'f';
                }
                if (ind == EXT2_ROOT_INO-1 || ind > EXT2_GOOD_OLD_FIRST_INO-1) {
                    printf("[%d] type: %c size: %d links: %d blocks: %d\n", ind+1, dtype, inode[ind].i_size,
                           inode[ind].i_links_count, inode[ind].i_blocks);
                    printf("[%d] Blocks:", ind+1);
                    if (0 < inode[ind].i_blocks && inode[ind].i_blocks <= 24) {
                        for(int m=0; m < inode[ind].i_blocks/2; m++) {
                            printf(" %d", inode[ind].i_block[m]);
                        }
                    } else {
                        for(int m=0; m < 12; m++) {
                            printf(" %d", inode[ind].i_block[m]);
                        }

                        int *location = (int *)(disk + 1024*inode[ind].i_block[12]);
                        for(int n=0; n < inode[ind].i_blocks/2 - 12-1; n++) {
                            printf(" %d", *(location + n));
                        }
                    }
                    printf("\n");
                }
            }
            ind ++;
        }

    }
    ind = 0;
    int count;
    printf("\n");
    printf("Directory Blocks:\n");
    for(int i = 0; i < sb->s_inodes_count/8; i++) {
        for(int j = 0; j < 8; j++) {
            if(inode_bitmap[i] & (1 << j)) {
                if (inode[ind].i_mode & EXT2_S_IFDIR) {
                    if (ind == EXT2_ROOT_INO-1 || ind > EXT2_GOOD_OLD_FIRST_INO-1) {
                        for(int k=0; k < inode[ind].i_blocks/2; k++) {
                            printf("   DIR BLOCK NUM: %d (for inode %d)\n", inode[ind].i_block[k], ind + 1);
                            struct ext2_dir_entry * dir_entry = (struct ext2_dir_entry *)(disk + inode[ind].i_block[k]*EXT2_BLOCK_SIZE);
                            count = 0;
                            while (count < EXT2_BLOCK_SIZE) {
                                if (EXT2_FT_REG_FILE == dir_entry->file_type) {
                                    printf("Inode: %d rec_len: %d name_len: %d type= f name=%.*s\n",
                                           dir_entry->inode, dir_entry->rec_len, dir_entry->name_len,dir_entry->name_len,dir_entry->name);
                                }
                                else if (EXT2_FT_DIR == dir_entry->file_type) {
                                    printf("Inode: %d rec_len: %d name_len: %d type= d name=%.*s\n",
                                           dir_entry->inode, dir_entry->rec_len, dir_entry->name_len,dir_entry->name_len,dir_entry->name);
                                }
                                count += dir_entry->rec_len;
                                dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
                            }
                        }

                    }

                }

            }

            ind ++;
        }

    }

    return 0;
}