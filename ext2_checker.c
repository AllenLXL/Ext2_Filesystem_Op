#include "ext2_utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image file name>", argv[0]);
        exit(1);
    }
    init_ptrs(argv[1]);
    int change = 0;


    // checker 3 to you must check that its inode is marked as
    // allocated in the inode bitmap. trust dir_ent->indoe
    for (int i = 0; i < sb->s_inodes_count; i++) {
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO) {
            // if inode_table[i] is a directory/file/link, I check
            // the inode bitmap to see if bitmap of it is 1. If not,
            // correct it.
            if (inode_table[i].i_size > 0) {
                if (get_bitmap(0, i + 1) != 1) {
                    change += 1;
                    set_bitmap(0, i + 1, 1);
                    sb->s_free_inodes_count--;
                    gdt->bg_free_inodes_count--;
                    printf("Fixed: inode [%d] not marked as in-use\n", i + 1);
                }
            }
        } else {
            // reserved block set to 1 if it's originally 0.
            if (get_bitmap(0, i + 1) != 1) {
                change += 1;
                set_bitmap(0, i + 1, 1);
                sb->s_free_inodes_count--;
                gdt->bg_free_inodes_count--;
                printf("Fixed: inode [%d] not marked as in-use\n", i + 1);
            }
        }
    }


    // update superblock and block group counters, trust bitmap
    // loop over block bitmap and inode bitmap to find the block in use and
    // inode in use.
    int block_num = 0;
    for (int byte = 0; byte < sb->s_blocks_count / 8; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            if (!(block_bm[byte] & (1 << bit))) {
                block_num++;
            }
        }
    }

    int inode_num = 0;
    for (int byte = 0; byte < sb->s_inodes_count / 8; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            if (!(inode_bm[byte] & (1 << bit))) {
                inode_num++;
            }
        }
    }
    // adjust blocks & inodes numbers by compare the actual free blocks and superblock
    // and block group table's free blocks/inodes count.
    if (block_num != sb->s_free_blocks_count) {
        change = abs((int)sb->s_free_blocks_count - block_num);
        sb->s_free_blocks_count = (unsigned int) block_num;
        printf("Fixed: superblock's free blocks counter was off by %d compared to the bitmap\n", change);
    }
    if (block_num != gdt->bg_free_blocks_count) {
        change = abs((int)gdt->bg_free_blocks_count - block_num);
        printf("%d, %d\n", gdt->bg_free_blocks_count, block_num);
        gdt->bg_free_blocks_count = (unsigned short) block_num;
        printf("Fixed: block group's free blocks counter was off by %d compared to the bitmap\n", change);
    }
    if (inode_num != sb->s_free_inodes_count) {
        change = abs((int)sb->s_free_inodes_count - inode_num);
        sb->s_free_inodes_count = (unsigned int) inode_num;
        printf("Fixed: superblock's free inodes counter was off by %d compared to the bitmap\n", change);
    }
    if (inode_num != gdt->bg_free_inodes_count) {
        change = abs((int)gdt->bg_free_inodes_count - inode_num);
        gdt->bg_free_inodes_count = (unsigned short) inode_num;
        printf("Fixed: block group's free inodes counter was off by %d compared to the bitmap\n", change);
    }

    // traverse all dir to check all dir ent, trust inode->imode
    for (int j = 0; j < sb->s_inodes_count; j++) {
        if (j == EXT2_ROOT_INO - 1 || j > EXT2_GOOD_OLD_FIRST_INO) {
            // if inode table[j] is a directory, we check every directory entry
            // under that folder to see if that directory entry match inode.
            if (inode_table[j].i_mode & EXT2_S_IFDIR) {
                change += check_files_in_dir(j);
            }
        }
    }


    // check d time by loop over inode tables. change every file, directory, and links'
    // dtime to 0 if it's originally 1.
    for (int k = 0; k < sb->s_inodes_count; k++) {
        if (((inode_table[k].i_mode & EXT2_S_IFREG) || (inode_table[k].i_mode & EXT2_S_IFDIR) ||
             (inode_table[k].i_mode & EXT2_S_IFLNK)) && inode_table[k].i_dtime != 0 && get_bitmap(0, k+1)==1) {
            change += 1;
            printf("Fixed: valid inode marked for deletion: [%d]\n", k + 1);
            inode_table[k].i_dtime = 0;
        }
    }
    // loop over inode table to make sure each blocks use by inode mark as inuse in block bitmap.
    for (int l = 0; l < sb->s_inodes_count; l++) {
        if (get_bitmap(0, l+1)==1) {
            change += check_blocks(l);
        }
    }


    if (!change) {
        printf("No file system inconsistencies detected!\n");
    } else{
        printf("%d file system inconsistencies repaired!\n", change);
    }
}