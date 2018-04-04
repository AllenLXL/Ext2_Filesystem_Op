#include "ext2_utils.h"

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
    int type = check_type(dir_ent, name);
//    /*if (!type){
//        fprintf(stderr, "File to delete not exist\n");
//        exit(ENOENT);
//    } else */
    if (type==2){
        fprintf(stderr, "Cannot restore a directory\n");
        exit(EISDIR);
    }

//    struct ext2_inode* parent_inode = &inode_table[dir_ent->inode-1];


    if (strncmp(dir_ent->name, name, dir_ent->name_len)==0){
        fprintf(stderr, "It cannot be restored.\n");
        return 0;
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
            int padding=0;
            int name_len = loop->dir_ent->name_len;
            while (name_len%4!=0){
                padding++;
                name_len++;
            }
//            int offset = (8+loop->dir_ent->name_len+padding);
//            struct ext2_dir_entry* prev = loop->dir_ent;
//            prev->rec_len= (8 + padding + prev->name_len);
            struct ext2_dir_entry* candidate = (void*)loop->dir_ent + (8+loop->dir_ent->name_len+padding);
            if (strncmp(candidate->name, name, candidate->name_len)==0){
                loop->dir_ent->rec_len= (unsigned short) (8 + loop->dir_ent->name_len + padding);
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
