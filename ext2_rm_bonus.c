/*
 * Implementing rmdir / rm -rf liked operation
 * Created by LiAllen on 2018-04-01.
 */

#include "ext2_utils.h"

int main(int argc, char **argv) {
    // trivial init stuff
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <abs path of ext2>\n", argv[0]);
        exit(1);
    }
    validate_path(argv[2]);

    init_ptrs(argv[1]);

    // init global link list
    first_front=NULL;
    construct_ll(argv[2], &first_front);

    char* name = get_last_name(first_front);

    struct ext2_dir_entry* dir_ent = get_parent_dir_block(first_front);
    struct ext2_inode* parent_inode = &inode_table[dir_ent->inode-1];
    int type = check_type(dir_ent, name);
    // error handling
    if (!type){
        fprintf(stderr, "File to delete not exist\n");
        exit(ENOENT);
    } else if (type==2){
        // rm dir bonus case, special treatment
        dir_ll_head=NULL;
        constrcut_dir_ll(dir_ent);
        dir_ll* loop = dir_ll_head;
        while (strncmp(loop->next->dir_ent->name, name, loop->next->dir_ent->name_len)!=0){
            loop=loop->next;
        }
        loop->dir_ent->rec_len+=loop->next->dir_ent->rec_len;
        parent_inode->i_links_count--;
        release_all(loop->next->dir_ent->inode);

        // free all malloc data
        free(name);
        free_ll(first_front);
        free_dir_ll(dir_ll_head);
        return 0;
    }

    // if we are remove the first dir ent
    if (strncmp(dir_ent->name, name, dir_ent->name_len)==0){
        set_bitmap(0, dir_ent->inode, 0);
        sb->s_free_inodes_count++;
        gdt->bg_free_inodes_count++;
        dir_ent->inode=0;

        // free all malloc data
        free(name);
        free_ll(first_front);
        free_dir_ll(dir_ll_head);
        return 0;
    }
    // link list for dir ent structure
    dir_ll_head=NULL;
    constrcut_dir_ll(dir_ent);

    // loop until we find the one we want to rm
    dir_ll* loop = dir_ll_head;
    while (strncmp(loop->next->dir_ent->name, name, loop->next->dir_ent->name_len)!=0){
        loop=loop->next;
    }
    loop->dir_ent->rec_len+=loop->next->dir_ent->rec_len;
    loop->next->dir_ent->rec_len= (unsigned short) get_rec_len(loop->next->dir_ent);

    int rm_inode_idx=loop->next->dir_ent->inode;

    // situation for hard link
    struct ext2_inode* deleted = &inode_table[rm_inode_idx-1];
    if (deleted->i_links_count>1){
        deleted->i_links_count--;

        // free all malloc data
        free(name);
        free_ll(first_front);
        free_dir_ll(dir_ll_head);
        return 0;
    }

    // modifying bm sb and gdt
    set_bitmap(0, rm_inode_idx, 0);
    sb->s_free_inodes_count++;
    gdt->bg_free_inodes_count++;

    // rm all blocks
    for (int i=0; i<inode_table[rm_inode_idx-1].i_blocks/2&&i<12;i++){
        set_bitmap(1, inode_table[rm_inode_idx-1].i_block[i], 0);
        sb->s_free_blocks_count++;
        gdt->bg_free_blocks_count++;
    }
    if (inode_table[rm_inode_idx-1].i_blocks/2 > 12){
        sb->s_free_blocks_count++;
        gdt->bg_free_blocks_count++;
        set_bitmap(1, inode_table[rm_inode_idx-1].i_block[12], 0);
        int *indirection = (int *)(disk + ((inode_table[rm_inode_idx-1].i_block[12]) * EXT2_BLOCK_SIZE));
        for (int i=0; i<inode_table[rm_inode_idx-1].i_blocks/2 -13;i++){
            set_bitmap(1, indirection[i], 0);
            sb->s_free_blocks_count++;
            gdt->bg_free_blocks_count++;
        }
    }

    // free all malloc data
    free(name);
    free_ll(first_front);
    free_dir_ll(dir_ll_head);
    return 0;
}