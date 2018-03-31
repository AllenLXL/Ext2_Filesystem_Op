#include "ext2_utils.h"

/*
 * the getter function is very similar to setter but it
 * returns the mode at the give idx. return -1 for error
 */
int get_bitmap(int bm_idx, int idx){
    idx --;
    int num_bit = 0;
    unsigned char* target_bm;
    if (!bm_idx){
        num_bit = sb->s_inodes_count;
        target_bm = inode_bm;
    } else{
        num_bit = sb->s_blocks_count;
        target_bm = block_bm;
    }
    for (int byte = 0; byte < num_bit / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if ((byte*8+bit) == idx){
                if ((target_bm[byte] & (1 << (bit)))>0){
                    return 1;
                }
                    return 0;
            }
        }
    }
    // code reach here indicates error
    exit(1);
}

int check_blocks(int inode_idx){
    int errors = 0;
    struct ext2_inode* inode = &inode_table[inode_idx];
    int block_need = inode->i_blocks/2;

    for (int i =0; i < block_need && i<12;i++){
        if (get_bitmap(1, i) == 0){
            set_bitmap(1, i, 1);
            sb->s_free_blocks_count--;
            gdt->bg_free_blocks_count--;
            errors ++;
        }
    }
    if (block_need>12){
        if (get_bitmap(1, inode->i_block[12]) == 0) {
            set_bitmap(1, inode->i_block[12], 1);
            sb->s_free_blocks_count--;
            gdt->bg_free_blocks_count--;
            errors ++;
        }
        int* location = (int *)(disk + EXT2_BLOCK_SIZE*inode[inode_idx].i_block[12]);
        for (int k = 0; k < block_need-13; k++) {
            if (get_bitmap(1, location[k]) == 0){
                set_bitmap(1, location[k], 1);
                sb->s_free_blocks_count--;
                gdt->bg_free_blocks_count--;
                errors ++;
            }
        }
    }
    printf("Fixed: %d in-use data blocks not marked in data bitmap for inode: [%d]\n", errors, inode_idx);
    return errors;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <image file name>", argv[0]);
        exit(1);
    }
    init_ptrs(argv[1]);

    // update superblock and block group counters, trust bitmap
    int block_num = 0;
    for (int byte = 0; byte < sb->s_blocks_count/8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (!(block_bm[byte] & (1 << bit))){
                block_num ++;
            }
        }
    }

    int inode_num = 0;
    for (int byte = 0; byte < sb->s_inodes_count / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (!(inode_bm[byte] & (1 << bit))){
                inode_num ++;
            }
        }
    }
    // adjust blocks & inodes numbers.
    int change = 0;
    if (block_num != sb->s_free_blocks_count) {
        change = abs(sb->s_free_blocks_count - block_num);
        sb->s_free_blocks_count = (unsigned int) block_num;
        printf("Fixed: superblock's free blocks counter was off by %d compared to the bitmap\n", change);
    }
    if (block_num != gdt->bg_free_blocks_count) {
        change = abs(gdt->bg_free_blocks_count - block_num);
        printf("%d, %d\n",gdt->bg_free_blocks_count, block_num);
        gdt->bg_free_blocks_count = (unsigned short) block_num;
        printf("Fixed: block group's free blocks counter was off by %d compared to the bitmap\n", change);
    }
    if (inode_num != sb->s_free_inodes_count) {
        change = abs(sb->s_free_inodes_count - inode_num);
        sb->s_free_inodes_count = (unsigned int) inode_num;
        printf("Fixed: superblock's free inodes counter was off by %d compared to the bitmap\n", change);
    }
    if (inode_num != gdt->bg_free_inodes_count) {
        change = abs(gdt->bg_free_inodes_count - inode_num);
        gdt->bg_free_inodes_count = (unsigned short) inode_num;
        printf("Fixed: block group's free inodes counter was off by %d compared to the bitmap\n", change);
    }

    // check if imode match file_type. If an Inode in inode_table
    // is a directory, we check everything under that folder.

    // traverse all dir to check all dir ent, trust inode->imode
    for (int i=EXT2_ROOT_INO-1;i<sb->s_inodes_count;i++){
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO){
            if (inode_table[i].i_mode & EXT2_S_IFDIR){
                change += check_files_in_dir(i);
            }
        }

    }

    // checker 3 to you must check that its inode is marked as
    // allocated in the inode bitmap. trust dir_ent->indoe
    printf("%d\n", get_bitmap(0, 1));
    for (int i=EXT2_ROOT_INO-1;i<sb->s_inodes_count;i++){
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO){
            if (inode_table[i].i_size > 0) {
                if (get_bitmap(0, i) != 1){
                    change += 1;
                    set_bitmap(0, i+1, 1);
                    sb->s_free_inodes_count--;
                    gdt->bg_free_inodes_count--;
                    printf("Fixed: inode [%d] not marked as in-use\n", i+1);
                }
            }
        }

    }

    for (int i=EXT2_ROOT_INO - 1;i<sb->s_inodes_count;i++){
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO){
            if (((inode_table[i].i_mode & EXT2_S_IFREG)||(inode_table[i].i_mode & EXT2_S_IFDIR)||(inode_table[i].i_mode & EXT2_S_IFLNK)) && inode_table[i].i_dtime != 0) {
                change += 1;
                printf("Fixed: valid inode marked for deletion: [%d]\n", i+1);
                inode_table[i].i_dtime = 0;
            }
        }

    }

    for (int i=EXT2_ROOT_INO - 1;i<sb->s_inodes_count;i++){
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO){
            if (inode_table[i].i_size>0){
                change += check_blocks(i);
            }
        }

    }
    if (!change){
        printf("No file system inconsistencies detected!\n");
    }
}