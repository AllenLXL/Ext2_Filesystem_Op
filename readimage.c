
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include "ext2.h"

unsigned char *disk;
/* number of inodes per block */
//unsigned int inodes_per_block = block_size / sizeof(struct ext2_inode);

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    static char r[9];
    for (int i=0; i<8;i++){
        r[i] = b[7-i];
        r[8]='\0';
    }
    return r;
}


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
    struct ext2_group_desc *gc = (struct ext2_group_desc *)(disk+2048);
    printf("Inodes: %d\n", sb->s_inodes_count);
    printf("Blocks: %d\n", sb->s_blocks_count);

    printf("Block group:\n");
    printf("    block bitmap: %d\n",gc->bg_block_bitmap);
    printf("    inode bitmap: %d\n",gc->bg_inode_bitmap);
    printf("    inode table: %d\n",gc->bg_inode_table);
    printf("    free blocks: %d\n",gc->bg_free_blocks_count);
    printf("    free inodes: %d\n",gc->bg_free_inodes_count);
    printf("    used_dirs: %d\n",gc->bg_used_dirs_count);

    printf("Block bitmap:");
    for (int i=0;i<16;i++){
        unsigned int bb = *(disk+gc->bg_block_bitmap*1024+i);
        printf(" ");
        printf("%s",byte_to_binary(bb));
    }
    printf("\n");
    printf("Inode bitmap: ");
    for (int i=0;i<4;i++){
        unsigned int bb = *(disk+ gc->bg_inode_bitmap*1024 +i);
        printf(" ");
        printf("%s",byte_to_binary(bb));
    }
    printf("\n\nInodes:\n");

    int i =0;
    for (i=0; i<sb->s_inodes_count;i++){
        struct ext2_inode * ei = (struct ext2_inode *)(disk + gc->bg_inode_table*1024+i*sb->s_inode_size);
        if (S_ISREG(ei->i_mode)||S_ISDIR(ei->i_mode)){
            if (ei->i_size >= 67383296) {continue;}
            if (ei->i_size == 12288) {continue;}
            if (ei->i_size <= 0) {continue;}
            char type;
            if (S_ISREG(ei->i_mode)){type='f';}
            else if (S_ISDIR(ei->i_mode)) {type='d';}
            printf("[%d] type: %c size: %u links: %d blocks: %d\n",
                   i+1, type, ei->i_size, ei->i_links_count, ei->i_blocks);

            printf("[%d] Blocks: ",i+1);
            int k; int check = 1;
            for (k=0;k<15;k++){
                if (ei->i_block[k]!=0 && k<12){
                    printf(" %d", ei->i_block[k]);
                    printf("\n");
                }
                else if (ei->i_block[k]==0 && k< 12){
                    check = 0;
                }
                if (k>=12 && check){
                    printf(" %d", ei->i_block[k]);
                    printf("\n");
                }

            }
        }

    }
    printf("\nDirectory Blocks:\n");
    for (i=0; i<sb->s_inodes_count;i++){
        struct ext2_inode * ei = (struct ext2_inode *)(disk + gc->bg_inode_table*1024+i*sb->s_inode_size);
        if (S_ISREG(ei->i_mode)||S_ISDIR(ei->i_mode)){
            if (ei->i_size >= 67383296) {continue;}
            if (ei->i_size == 12288) {continue;}
            if (ei->i_size <= 0) {continue;}
            char type;
            if (S_ISREG(ei->i_mode)){type='f';}
            else if (S_ISDIR(ei->i_mode)) {type='d';
                int k;
                for (k=0;k<15;k++){
                    if (ei->i_block[k]!=0){
                        printf("    DIR BLCOK NUM: %d (for inode %d)\n",ei->i_block[k],i+1);
                        struct ext2_dir_entry * dir_ent = (struct ext2_dir_entry *)(disk + ei->i_block[k]*EXT2_BLOCK_SIZE);
                        int exceed = 0;
                        while (exceed < EXT2_BLOCK_SIZE){
                            if (S_ISREG(dir_ent->file_type)){
                                type='f';
                            }
                            else if (S_ISDIR(dir_ent->file_type)){
                                type='d';
                            }
                            char * name;
                            name = dir_ent->name;
                            name[dir_ent->name_len]='\0';
                            printf("Inode: %d rec_len: %d name_len: %d type= %c name=%s\n",
                                   dir_ent->inode, dir_ent->rec_len, dir_ent->name_len,type,name);
                            exceed += dir_ent->rec_len;
                            dir_ent = (void*)(dir_ent) + dir_ent -> rec_len;
                        }
                    }
                }
            }
        }
    }
    return 0;
}