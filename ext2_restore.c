/*
 * Implementing restore operations
 * Created by LiAllen on 2018-03-19.
 */

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


    dir_ll_head=NULL;
    constrcut_dir_ll(dir_ent);
    dir_ll* loop = dir_ll_head;
    int offset=0;

    while (loop!=NULL){

        // this dir ent cannot contains removed dir ent since too less scace
        if(loop->dir_ent->rec_len - (8+loop->dir_ent->name_len) <= 3){
            if (loop->new_block==1){
                offset=0;
                offset+=loop->dir_ent->rec_len;
            } else{
                offset+=loop->dir_ent->rec_len;
            }
            loop=loop->next;
        } else{
            int rec_len=get_rec_len(loop->dir_ent);

            int inner_offset = get_rec_len(loop->dir_ent);
            struct ext2_dir_entry* candidate = (void*)loop->dir_ent + (rec_len);

            while (strncmp(candidate->name, name, candidate->name_len)!=0){
                if (candidate->name_len==0){
                    break;
                }
                int cur_rec_len = get_rec_len(candidate);
                candidate = (void*) candidate + cur_rec_len;
                inner_offset+=cur_rec_len;
                break;
            }
            if (candidate->name_len==0){
                if (loop->new_block==1){
                    offset=0;
                } else{
                    offset+=loop->dir_ent->rec_len;
                }
                loop=loop->next;
            }else{
                if (strncmp(candidate->name, name, candidate->name_len)==0){
                    // we find target however it's a dir, error case

                    if (candidate->file_type==EXT2_FT_DIR){
                        fprintf(stderr, "cannot restore directory\n");
                        exit(EISDIR);
                    }
                    int true_rec_len=0;
//                    struct ext2_dir_entry* if_next = (void*) candidate + get_rec_len(candidate);
                    //resotre last dir ent
                    if (loop->next==NULL || loop->next->new_block==1){
                        true_rec_len=EXT2_BLOCK_SIZE-(offset+inner_offset);
                    } else{ // restore not last entry
                        true_rec_len=loop->dir_ent->rec_len-inner_offset;
                    }
                    candidate->rec_len= (unsigned short) true_rec_len;
                    loop->dir_ent->rec_len= (unsigned short) (inner_offset);
                    int rs_inode_idx = candidate->inode;
                    struct ext2_inode* rs_inode = &inode_table[rs_inode_idx-1];
                    rs_inode->i_dtime= 0;
                    set_bitmap(0, rs_inode_idx, 1);
                    sb->s_free_inodes_count--;
                    gdt->bg_free_inodes_count--;

                    //restoring data block
                    for (int i=0; i<rs_inode->i_blocks/2&&i<12; i++){
                        set_bitmap(1, rs_inode->i_block[i], 1);
                        sb->s_free_blocks_count--;
                        gdt->bg_free_blocks_count--;
                    }   // case for a large file
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
                    // successfully restore!
                    return 0;
                }
            }
        }


    }
    // code reaches here indicates it cannot restore file
    // since it cannot find it
    fprintf(stderr, "cannot restore file\n");
    exit(ENOENT);
}
