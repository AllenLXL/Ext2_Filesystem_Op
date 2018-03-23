//
// Created by LiAllen on 2018-03-19.
//


#include "ext2_utils.h"
#include <stdlib.h>

/*
 * This function is for initialize all necessary pointer for every operations.
 * On success return image file descriptor, o.w. exit.
 */
int init_ptrs(char* img_file){
    int fd = open(img_file, O_RDWR);

    if (fd < 0){
        perror("Error opening file.");
        exit(ENOENT);
    }

    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    sb = (struct ext2_super_block *)(disk + EXT2_BLOCK_SIZE);
    gdt = (struct ext2_group_desc *)(disk + EXT2_BLOCK_SIZE * 2);
    block_bm = disk + (EXT2_BLOCK_SIZE * gdt->bg_block_bitmap);
    inode_bm = disk + (EXT2_BLOCK_SIZE * gdt->bg_inode_bitmap);
    inode_table = (struct ext2_inode *)(disk + EXT2_BLOCK_SIZE * gdt->bg_inode_table);

    return fd;
}
/*
 * This function is used for finding a free inode in inode bitmap.
 * If there is no free inode now, return -1 indicating error.
 */
int find_free_inode(){
    int idx = 0;
    for (int byte = 0; byte < sb->s_inodes_count / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (inode_bm[byte] & (1 << bit)){
                idx++;
            } else{
                return idx;
            }
        }
    }
    return -1;
}

/*
 * This function is used for finding a free block in block bitmap.
 * If there is no free block now, return -1 indicating error.
 */
int find_free_block(){
    int idx = 0;
    for (int byte = 0; byte < sb->s_blocks_count / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (block_bm[byte] & (1 << bit)){
                idx++;
            } else{
                return idx;
            }
        }
    }
    return -1;
}

/*
 * This function is used to set bits of inode/block bitmap on or off.
 * bm_idx can be 0 or 1 indicates inode bitmap or block bitmap
 * idx indicates bit index in bitmap. if mode is 0, turn idx on, o.w. off.
 */
void set_bitmap(int bm_idx ,int idx, int mode){
    idx --;
    int num_bit;
    if (!bm_idx){
        num_bit = sb->s_blocks_count;
    } else{
        num_bit = sb->s_inodes_count;
    }
    int cur_idx = 0;

    for (int byte = 0; byte < num_bit / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (cur_idx == idx){
                if (mode){
                    block_bm[byte] = (unsigned char) (block_bm[byte] | (1 << bit));
                } else{
                    int temp = 0xff;
                    temp = temp ^ (1 << bit);
                    block_bm[byte] = (unsigned char) (block_bm[byte] & temp);
                }
            }
        }
    }
}

/*
 * This helper function works for converting an abs path to
 * some normal staff. Path is guaranteed to be null terminated.
 * make sure a slash in front and no slash in end.
 */
char* convert_path(char* path){
    int length = 0;
    while (path[length] != '\0'){
        length++;
    }

    int front_slash = 0;
    if (path[0] == '/'){
        front_slash = 1;
    }

    int end_slash = 0;
    if (path[length-1] == '/'){
        end_slash = 1;
    }
    int new_length = length;
    if (!front_slash){
        new_length++;
    }

    if (!end_slash){
        new_length++;
    }

    char* good_path = malloc(new_length*sizeof(char));
    memset(good_path, '\0', new_length);
    if (!front_slash){
        good_path[0] = '/';
        strncpy(&good_path[1], path, new_length);
        if (end_slash){
            good_path[new_length-1] = '\0';
        }
    } else{
        strncpy(good_path, path, new_length);
        if (end_slash){
            good_path[new_length-1] = '\0';
        }
    }
    return good_path;
}

void construct_ll(char* path){
    front = NULL;

    ll_length = 0;
    int length = 0;
    while (path[length] != '\0'){
        length++;
    }
    int dir_length = 0;
    for(int i = length - 1; i >= 1; i--){
        if (path[i] != '/'){
            dir_length++;
        } else{
            dir_length = 0;
        }
        if (path[i]!= '/' && path[i-1] == '/'){
            ll* ll_node = malloc(sizeof(ll));
            ll_node->name = malloc(dir_length* sizeof(char));
            memcpy(ll_node->name, &path[i], dir_length);
            ll_node->next = front;
            ll_node->name_len = dir_length;
            ll_length++;
        }
    }
    free(path);
}


/*
 * This is a helper function for check if a given path is abs.
 * NOTE path should be null terminated.
 * On success return 1. o.w. exit.
 * If valid, we remove last / if exist.
 */
int validate_abs_path(char* path){
    if (path[0]!='/'){
        fprintf(stderr, "invalid abs path");
        exit(1);
    }
    return 1;
}

/*
 * This is a helper function for return new dir's name if possible. o.w. exit.
 * NOTE path should be null terminated.
 */
char* get_new_dir_name(char* path){
    int length = 0;
    while (path[length] != '\0'){
        length++;
    }
    char path_cpy[length+1];
    memset(path_cpy, '\0', length+1);
    strncpy(path_cpy, path, length);

    // remove last slash if exist
    if (path_cpy[length-1] == '/'){
        path_cpy[length-1] = '\0';
        length--;
    }

    int name_length = 0;
    for (int i = length-1; i >= 0; i--){
        if (path_cpy[i]!= '/'){
            name_length++;
        } else{
            break;
        }
    }
    if (name_length == 0){
        fprintf(stderr, "function proceed to this line indicates errors.");
        exit(1);
    }

    char* new_dir_name = malloc((name_length+1) * sizeof(char));
    memset(new_dir_name, '\0', name_length+1);
    strncpy(new_dir_name ,&path_cpy[length-name_length], name_length);
    return new_dir_name;
}

// return parent inode idx
unsigned int modify_parent_block(){
    struct ext2_inode* root = &inode_table[EXT2_ROOT_INO - 1];
    struct ext2_inode* current = root;
    struct ext2_dir_entry * dir_entry = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE);

    ll* current_node = front;

    int k = 0;
    for (int i = 0; i < ll_length - 1; i++){

        current_node = current_node->next;
        while (k < EXT2_BLOCK_SIZE) {
            if (EXT2_FT_REG_FILE == dir_entry->file_type) {
                k += dir_entry->rec_len;
                dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
            }
            else if (EXT2_FT_DIR == dir_entry->file_type) {
                if (strncmp(dir_entry->name, current_node->name, (size_t) current_node->name_len) == 0){
                    current = &inode_table[dir_entry->inode - 1];
                    dir_entry = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE);
                    k = 0;
                    current_node = current_node->next;
                    if (i == ll_length-1){
                        break;
                    }
                } else{
                    fprintf(stderr, "directory not exist");
                    exit(ENOENT);
                }
            }
            k += dir_entry->rec_len;
            dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
        }
    }

    unsigned int result = dir_entry->inode;

    while (k < EXT2_BLOCK_SIZE) {
        if (k+dir_entry->rec_len == EXT2_BLOCK_SIZE){
            break;
        }
        k += dir_entry->rec_len;
        dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;

    }
    int last_padding = EXT2_BLOCK_SIZE - k - 8 - dir_entry->name_len;
    if (last_padding < (current_node->name_len+8)){
        fprintf(stderr, "no enough space in this block, gg.");
        exit(1); //TODO exit code?
    }

    struct ext2_dir_entry * new_dir = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE + k + 8 + dir_entry->name_len);
    new_dir->name_len= (unsigned char) current_node->name_len;
    strncpy(new_dir->name, current_node->name, current_node->name_len);
    new_dir->rec_len= (unsigned short) (EXT2_BLOCK_SIZE - k);
    new_dir->inode= (unsigned int) (find_free_inode() + 1);
    new_dir->file_type=EXT2_FT_DIR;

    return result;
}
