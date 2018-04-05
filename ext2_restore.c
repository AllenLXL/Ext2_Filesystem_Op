/*
 * Implementing restore operations
 * Created by LiAllen on 2018-03-19.
 */

#include "ext2_utils.h"

int main(int argc, char** argv){
    // some trivial validation and init stuff
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <abs path of ext2>\n", argv[0]);
        exit(1);
    }

    validate_path(argv[2]);

    init_ptrs(argv[1]);

    // init global link list to NULL and construct it
    first_front=NULL;
    construct_ll(argv[2], &first_front);

    // get target name
    char* name = get_last_name(first_front);

    struct ext2_dir_entry* dir_ent = get_parent_dir_block(first_front);

    // init global link list to NULL and build it
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

            int inner_offset = 0; // offset btw loop->dir_ent and target dir ent if exist
            struct ext2_dir_entry* candidate = (void*)loop->dir_ent + (rec_len);

            int extra_space = loop->dir_ent->rec_len-get_rec_len(loop->dir_ent);
            // target rs file is possible in candidate padding
            while (strncmp(candidate->name, name, candidate->name_len)!=0 && inner_offset<extra_space){
                int cur_rec_len = get_rec_len(candidate);
                candidate = (void*) candidate + cur_rec_len;
                inner_offset+=cur_rec_len;
            }
            // if we go to somewhere in padding, doesn't make any sense,
            // simply loop to next loop
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

                    //resotre last dir ent (with long padding)
                    if (loop->next==NULL || loop->next->new_block==1){
                        true_rec_len=EXT2_BLOCK_SIZE-(offset+inner_offset+get_rec_len(loop->dir_ent));
                    } else{ // restore not last entry (with short padding)
                        true_rec_len=loop->dir_ent->rec_len-inner_offset-get_rec_len(loop->dir_ent);
                    }

                    // update necessary infomation
                    candidate->rec_len= (unsigned short) true_rec_len;
                    loop->dir_ent->rec_len= (unsigned short) (inner_offset+get_rec_len(loop->dir_ent));
                    int rs_inode_idx = candidate->inode;
                    struct ext2_inode* rs_inode = &inode_table[rs_inode_idx-1];
                    rs_inode->i_dtime= 0;
                    rs_inode->i_links_count=1;

                    // set bitmap related info
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
                } else{
                    // we go to next loop and check again
                    if (loop->new_block==1){
                        offset=0;
                    } else{
                        offset+=loop->dir_ent->rec_len;
                    }
                    loop=loop->next;
                }
            }
        }
    }
    // code reaches here indicates it cannot restore file
    // since it cannot find it
    fprintf(stderr, "cannot restore file\n");
    exit(ENOENT);
}
