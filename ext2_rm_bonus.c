/*
 * Implementing rmdir / rm -rf liked operation
 * Created by LiAllen on 2018-04-01.
 */

#include "ext2_utils.h"

int freed[128];
memset(freed, 0, 128);

dir_ll* constrcut_dir_ll_spe(struct ext2_dir_entry* dir_entry, dir_ll* head){
    struct ext2_inode* inode = &inode_table[dir_entry->inode-1];

    head=NULL;
    dir_ll* cur_dir_ll; // loop invariant
    int k =0;
    for (int i=0; i < inode->i_blocks/2;i++){
        k=0;
        dir_entry= (struct ext2_dir_entry *) (disk + EXT2_BLOCK_SIZE * inode->i_block[i]);
        while (k < EXT2_BLOCK_SIZE) {
            cur_dir_ll =malloc(sizeof(dir_ll));
            cur_dir_ll->dir_ent=dir_entry;
            cur_dir_ll->next=head;
            head = cur_dir_ll;

            k += dir_entry->rec_len;
            dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
        }

    }

    // now reverse this linked list
    dir_ll* prev = NULL;
    dir_ll* cur = head;
    dir_ll* next = NULL;

    while (cur!=NULL){
        next=cur->next;
        cur->next=prev;
        prev=cur;
        cur=next;
    }

    head=prev;
    return head;
}

// inode_idx starts from 1
void release_all(int inode_idx){
    set_bitmap(0,inode_idx,0);
    sb->s_free_inodes_count++;
    gdt->bg_free_blocks_count++;
    struct ext2_inode* current = &inode_table[inode_idx-1];
    if (current->i_mode & EXT2_S_IFDIR){
        gdt->bg_used_dirs_count--;
    }
    int block_used = current->i_blocks/2;
    for (int i=0;i<block_used;i++){
        set_bitmap(1,current->i_block[i],0);
        sb->s_free_blocks_count++;
        gdt->bg_free_blocks_count++;
    }
    if (current->i_mode & EXT2_S_IFDIR){

        dir_ll* head= NULL;

        struct ext2_dir_entry* dir_ent = (struct ext2_dir_entry*) (disk + EXT2_BLOCK_SIZE*current->i_block[0]);
        head = constrcut_dir_ll_spe(dir_ent, head);

        while(head!=NULL && (strncmp(head->dir_ent->name, ".", 1)!=0) && (strncmp(head->dir_ent->name, "..", 2)!=0)){
            release_all(head->dir_ent->inode);
            head=head->next;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <abs path of ext2>\n", argv[0]);
        exit(1);
    }
    validate_path(argv[2]);

    init_ptrs(argv[1]);

    first_front=NULL;
    construct_ll(argv[2], &first_front);

    char* name = get_last_name(first_front);

    struct ext2_dir_entry* dir_ent = get_parent_dir_block(first_front);
    int type = check_type(dir_ent, name);
    if (!type){
        fprintf(stderr, "File to delete not exist\n");
        exit(ENOENT);
    } else if (type==2){
        dir_ll_head=NULL;
        constrcut_dir_ll(dir_ent);
        dir_ll* loop = dir_ll_head;
        while (strncmp(loop->next->dir_ent->name, name, loop->next->dir_ent->name_len)!=0){
            loop=loop->next;
        }
        loop->dir_ent->rec_len+=loop->next->dir_ent->rec_len;
        release_all(loop->next->dir_ent->inode);
        return 0;
    }

    if (strncmp(dir_ent->name, name, dir_ent->name_len)==0){
        set_bitmap(0, dir_ent->inode, 0);
        sb->s_free_inodes_count++;
        gdt->bg_free_inodes_count++;
        dir_ent->inode=0;
        return 0;
    }

    dir_ll_head=NULL;
    constrcut_dir_ll(dir_ent);

    dir_ll* loop = dir_ll_head;
    while (strncmp(loop->next->dir_ent->name, name, loop->next->dir_ent->name_len)!=0){
        loop=loop->next;
    }
    loop->dir_ent->rec_len+=loop->next->dir_ent->rec_len;

    int rm_inode_idx=loop->next->dir_ent->inode;
    set_bitmap(0, rm_inode_idx, 0);
    sb->s_free_inodes_count++;
    gdt->bg_free_inodes_count++;
    // situation for hard link
    struct ext2_inode* deleted = &inode_table[rm_inode_idx-1];
    if (deleted->i_links_count>1){
        deleted->i_links_count--;
    }

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
    return 0;
}