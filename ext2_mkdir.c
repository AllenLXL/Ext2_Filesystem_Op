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
    int free_inode_idx = find_free_inode() + 1;
    int free_block_idx = find_free_block() + 1;

    struct ext2_inode* new_inode = &inode_table[free_inode_idx - 1];

    new_inode->i_mode = EXT2_S_IFDIR;
    new_inode->i_size = EXT2_BLOCK_SIZE;
    new_inode->i_links_count = 2;
    new_inode->i_blocks = 2;
    // the following are trivial staff
    new_inode->i_gid = 0;
    new_inode->i_uid = 0;
    new_inode->osd1 = 0;
    new_inode->i_generation = 0;
    new_inode->i_file_acl = 0;    /* File ACL */
    new_inode->i_dir_acl = 0;     /* Directory ACL */
    new_inode->i_faddr = 0;

    new_inode->i_block[0] = (unsigned int) free_block_idx;
    int i;
    for(i = 1; i < 15; i++) {
        new_inode->i_block[i] = 0;
    }

    unsigned int parent_idx = modify_parent_block() - 1;

    //set first dir_entry to itself
    struct ext2_dir_entry_2 *new_dir_entry = (struct ext2_dir_entry_2 *)(disk + (free_block_idx * EXT2_BLOCK_SIZE));
    new_dir_entry->inode = free_inode_idx;
    new_dir_entry->rec_len = 12;
    new_dir_entry->name_len = 1;
    new_dir_entry->file_type = EXT2_FT_DIR;
    strncpy(new_dir_entry->name, ".", 1);

    //set another dir_entry to = parent
    new_dir_entry = (struct ext2_dir_entry_2 *)((char *)new_dir_entry + new_dir_entry->rec_len);
    new_dir_entry->inode = ((struct ext2_dir_entry_2 *)(disk + (inode_table[parent_idx].i_block[0] * EXT2_BLOCK_SIZE)))->inode;
    new_dir_entry->rec_len = 12;
    new_dir_entry->name_len = 2;
    new_dir_entry->file_type = EXT2_FT_DIR;
    strncpy(new_dir_entry->name, "..", 2);
    inode_table[parent_idx].i_links_count++;

    // switch bitmap off
    set_bitmap(0, free_inode_idx-1, 0);
    set_bitmap(1, free_block_idx-1, 0);

}