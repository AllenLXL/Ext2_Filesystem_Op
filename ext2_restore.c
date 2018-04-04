#include "ext2_utils.h"
int get_rec_len(struct ext2_dir_entry* dir_ent){
    int result = 8;
    result+=dir_ent->name_len;
    while (result%4!=0){
        result++;
    }
    return result;
}


int main(int argc, char** argv){
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

    if (strncmp(dir_ent->name, name, dir_ent->name_len)==0){
        fprintf(stderr, "It cannot be restored.\n");
        exit(1);
    }

    dir_ll_head=NULL;
    constrcut_dir_ll(dir_ent);
    dir_ll* loop = dir_ll_head;

    while (loop!=NULL){
//        char buf[EXT2_FT_MAX];
//        memset(buf, '\0', EXT2_FT_MAX);
//        memcpy(buf, loop->dir_ent->name, loop->dir_ent->name_len);
//        printf("loop is %s\n", buf);
        if(loop->dir_ent->rec_len - (8+loop->dir_ent->name_len) <= 3){
            loop=loop->next;
        } else{
            int rec_len=get_rec_len(loop->dir_ent);


            struct ext2_dir_entry* candidate = (void*)loop->dir_ent + (rec_len);
            while (strncmp(candidate->name, name, candidate->name_len)!=0){
                int true_rec_len = get_rec_len(candidate);
                candidate = (void*) candidate + true_rec_len;
            }
            if (strncmp(candidate->name, name, candidate->name_len)==0){
                loop->dir_ent->rec_len= (unsigned short) (rec_len);
                int rs_inode_idx = candidate->inode;
                struct ext2_inode* rs_inode = &inode_table[rs_inode_idx-1];
                rs_inode->i_links_count++;
                rs_inode->i_dtime= 0;
                set_bitmap(0, rs_inode_idx, 1);
                sb->s_free_inodes_count--;
                gdt->bg_free_inodes_count--;
                for (int i=0; i<rs_inode->i_blocks/2&&i<12; i++){
                    set_bitmap(1, rs_inode->i_block[i], 1);
                    sb->s_free_blocks_count--;
                    gdt->bg_free_blocks_count--;
                }
                if (rs_inode->i_blocks/2 > 12){
                    set_bitmap(1, rs_inode->i_block[12], 1);
                    sb->s_free_blocks_count--;
                    gdt->bg_free_blocks_count--;
                    int* indirection = (int *)(disk + (rs_inode->i_block[12]) * EXT2_BLOCK_SIZE);
                    for (int i=0; i<rs_inode->i_blocks/2-13;i++){
                        set_bitmap(1, indirection[i], 1);
                        sb->s_free_blocks_count--;
                        gdt->bg_free_blocks_count--;
                    }
                }
                return 0;
            } else{
                loop=loop->next;
            }
        }
    }
    return 0;
}
