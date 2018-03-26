
#include "ext2_utils.h"

//TODO====================================================
/* Helper function for debugging purposes*/
void print_bm(){
    int in_use;
//    unsigned char *bb_content = disk + (EXT2_BLOCK_SIZE * gdt->bg_block_bitmap);
    printf("Block bitmap: ");
    for (int byte = 0; byte < 16; byte++){
        for (int bit = 0; bit < 8; bit++){
            in_use = block_bm[byte] & (1 << bit);
            if (in_use){
                printf("%d", 1);
            } else{
                printf("%d", in_use);
            }
        }
        if (byte!=16-1){
            printf(" ");
        }
    }
    printf("\n");
//    unsigned char *ib_content = disk + (EXT2_BLOCK_SIZE * gdt->bg_inode_bitmap);
    printf("Inode bitmap: ");

    for (int byte = 0; byte < 32 / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            in_use = inode_bm[byte] & (1 << bit);
            if (in_use){
                printf("%d", 1);
            } else{
                printf("%d", in_use);
            }
        }
        if (byte!=4-1){
            printf(" ");
        }
    }
    printf("\nbit map printing finishing\n");
}

void print_dir_block(struct ext2_dir_entry* first_row){
    int k = 0;
    while (k < EXT2_BLOCK_SIZE) {
        if (EXT2_FT_REG_FILE == first_row->file_type) {
            printf("Inode: %d rec_len: %d name_len: %d type= f name=%.*s\n",
                   first_row->inode, first_row->rec_len, first_row->name_len,first_row->name_len,first_row->name);
        }
        else if (EXT2_FT_DIR == first_row->file_type) {
            printf("Inode: %d rec_len: %d name_len: %d type= d name=%.*s\n",
                   first_row->inode, first_row->rec_len, first_row->name_len,first_row->name_len,first_row->name);
        }
        k += first_row->rec_len;
        first_row = (void*)(first_row) + first_row->rec_len;
    }
    printf("print finished ================================\n");
}

void print_inode(){
    printf("Inodes:\n");
    struct ext2_inode *inode = inode_table;

    for (int i = EXT2_ROOT_INO - 1; i < sb->s_inodes_count; i++) {
        if (i == EXT2_ROOT_INO - 1 || i >= EXT2_GOOD_OLD_FIRST_INO) {

            if (inode[i].i_size != 0) {
                if (inode[i].i_mode & EXT2_S_IFREG) { // regular
                    printf("[%d] type: f size: %d links: %d blocks: %d\n", i + 1, inode[i].i_size,
                           inode[i].i_links_count, inode[i].i_blocks);
                    printf("[%d] Blocks: ", i + 1);
                    if (inode[i].i_blocks / 2 < 12){
                        for (int j = 0; j < (inode[i].i_blocks / 2); j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        printf("\n");
                    } else {
                        for (int j = 0; j < 13; j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        int* location = (int *)(disk + EXT2_BLOCK_SIZE*inode[i].i_block[12]);
                        for (int k = 0; k < (inode[i].i_blocks / 2)-12-1; k++) {
                            if (k!=(inode[i].i_blocks / 2)-12-1-1){
                                printf("%d ", location[k]);
                            } else{
                                printf("%d", location[k]);
                            }
                        }
                        printf("\n");
                    }
                }
                if (inode[i].i_mode & EXT2_S_IFDIR) { // directory
                    printf("[%d] type: d size: %d links: %d blocks: %d\n", i + 1, inode[i].i_size,
                           inode[i].i_links_count, inode[i].i_blocks);
                    printf("[%d] Blocks: ", i + 1);
                    if (inode[i].i_blocks / 2 < 12){
                        for (int j = 0; j < (inode[i].i_blocks / 2); j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        printf("\n");
                    } else {
                        for (int j = 0; j < 13; j++) {
                            if (j!=(inode[i].i_blocks / 2)-1){
                                printf("%d ", inode[i].i_block[j]);
                            } else{
                                printf("%d", inode[i].i_block[j]);
                            }
                        }
                        int* location = (int *)(disk + EXT2_BLOCK_SIZE*inode[i].i_block[12]);
                        for (int k = 0; k < (inode[i].i_blocks / 2)-12-1; k++) {
                            if (k!=(inode[i].i_blocks / 2)-12-1-1){
                                printf("%d ", location[k]);
                            } else{
                                printf("%d", location[k]);
                            }
                        }
                        printf("\n");
                    }
                }
            }
        }
    }
}
//TODO====================================================

/*
 * This function is for initialize all necessary pointer for every operations.
 * On success return image file descriptor, o.w. exit.
 */
void init_ptrs(char* img_file){
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
                if (mode){
                    target_bm[byte] = (unsigned char) (target_bm[byte] | (1 << (bit)));
                    break;
                } else{
                    int temp = 0xff;
                    temp = temp ^ (1 << bit);
                    target_bm[byte] = (unsigned char) (target_bm[byte] & temp);
                    break;
                }
            }
        }
    }
}


void construct_ll(char* path, ll** link_list){
    *link_list = NULL;

    int length = (int) strlen(path);
    int dir_length = 0;
    for(int i = length - 1; i >= 1; i--){
        if (path[i] != '/'){
            dir_length++;
        } else{
            dir_length = 0;
        }
        if ((path[i]!= '/' && path[i-1] == '/')||(i==1 && dir_length!=0 && path[0]!='/')){
            if (i==1 && dir_length!=0 && path[0]!='/'){
                dir_length++;
                i--;
            }
            ll* ll_node = malloc(sizeof(ll));
            ll_node->name = malloc((dir_length+1)* sizeof(char));
            memset(ll_node->name, '\0', dir_length+1);
            memcpy(ll_node->name, &path[i], dir_length);
            ll_node->next = (*link_list);
            (*link_list) = ll_node;
            ll_node->name_len = dir_length;
        }
    }
}

int get_ll_length(ll* head){
    ll* temp = head;
    int result = 0;
    while(temp != NULL){
        result++;
        temp=temp->next;
    }
    return result;
}


/*
 * This is a helper function for check if a given path is abs.
 * NOTE path should be null terminated.
 * On success return 1. o.w. exit.
 * If valid, we remove last / if exist.
 */
void validate_path(char* path){
    if (path[0]!='/'){
        fprintf(stderr, "invalid abs path");
        exit(ENOENT);
    }
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

    ll* current_node = first_front;

    int ll_length = get_ll_length(first_front);

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
//    print_dir_block(dir_entry);

    unsigned int result = dir_entry->inode;

    struct ext2_dir_entry* need_deleted = dir_entry;

    while (k < EXT2_BLOCK_SIZE) {
        if (k+dir_entry->rec_len == EXT2_BLOCK_SIZE){
            int begin = k;
            int row_len = 8 + dir_entry->name_len;
            k+=row_len;
            while ((row_len)%4!=0){
                row_len++;
                k++;
            }
            dir_entry->rec_len = (unsigned short) (k - begin);
            break;
        }
        k += dir_entry->rec_len;
        dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
    }

    struct ext2_dir_entry * new_dir = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE+k);
    new_dir->name_len= (unsigned char) current_node->name_len;
    strncpy(new_dir->name, current_node->name, current_node->name_len);
    new_dir->rec_len= (unsigned short) (EXT2_BLOCK_SIZE - k);

    int last_padding = EXT2_BLOCK_SIZE - k - 8 - dir_entry->name_len;
    if (last_padding < (current_node->name_len+8)){
        fprintf(stderr, "no enough space in this block, gg.");
        exit(1); //TODO exit code?
    }

    new_dir->inode= (unsigned int) (find_free_inode() + 1);
    new_dir->file_type=EXT2_FT_DIR;

    print_dir_block(need_deleted);

    return result;
}

struct ext2_dir_entry* get_parent_dir_block(ll* link_list_head){
    struct ext2_inode* root = &inode_table[EXT2_ROOT_INO - 1];
    struct ext2_inode* current = root;
    struct ext2_dir_entry * dir_entry = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE);

    ll* current_node = link_list_head;

    int ll_length = get_ll_length(first_front);

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
    return dir_entry;
}

void add_parent_block(struct ext2_dir_entry* dir_entry, char* name, int type){

    unsigned int inode_idx = dir_entry->inode;
    struct ext2_inode* inode = &inode_table[inode_idx-1];
    struct ext2_dir_entry * new_dir;

    struct ext2_dir_entry* need_deleted = dir_entry;

    int k =0;
    while (k < EXT2_BLOCK_SIZE) {
        if (k+dir_entry->rec_len == EXT2_BLOCK_SIZE){
            int begin = k;
            int row_len = 8 + dir_entry->name_len;
            k+=row_len;
            while ((row_len)%4!=0){
                row_len++;
                k++;
            }
            dir_entry->rec_len = (unsigned short) (k - begin);
            break;
        }
        k += dir_entry->rec_len;
        dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
    }
    int last_padding = EXT2_BLOCK_SIZE - k - 8 - dir_entry->name_len;

    if (last_padding < (strlen(name)+8)){
        if (sb->s_free_blocks_count < 1) {
            fprintf(stderr, "No block available");
            exit(1);
        }
        inode->i_blocks += 2;
        int free_block_idx = find_free_block() + 1;
        inode->i_block[1]= (unsigned int) free_block_idx;
        set_bitmap(0, free_block_idx, 1);
        sb->s_free_blocks_count--;
        gdt->bg_free_inodes_count--;
        new_dir = (struct ext2_dir_entry *)(disk + inode->i_block[1]*EXT2_BLOCK_SIZE);
        k=0;
    } else{
        new_dir = (struct ext2_dir_entry *)(disk + inode->i_block[0]*EXT2_BLOCK_SIZE+k);
    }
    new_dir->name_len= (unsigned char) strlen(name);
    strncpy(new_dir->name, name, strlen(name));
    new_dir->rec_len= (unsigned short) (EXT2_BLOCK_SIZE - k);
    new_dir->inode= inode_idx;
    new_dir->file_type= (unsigned char) type;
}


void init_inode(struct ext2_inode* new_inode){
    // the following are trivial staff
    new_inode->i_gid = 0;
    new_inode->i_uid = 0;
    new_inode->osd1 = 0;
    new_inode->i_generation = 0;
    new_inode->i_file_acl = 0;    /* File ACL */
    new_inode->i_dir_acl = 0;     /* Directory ACL */
    new_inode->i_faddr = 0;
    // other necessary attributes
    new_inode->i_blocks=0;
    new_inode->i_links_count=0;
    for(int i =0;i<15;i++){
        new_inode->i_block[i]=0;
    }
}

/*
 * name is file / dir name.
 */
void check_existence(struct ext2_dir_entry* first_dir_ent , char* name, int type){
    int k = 0;
    while (k < EXT2_BLOCK_SIZE) {
        if (type == first_dir_ent->file_type && (strncmp(name, (const char *) (first_dir_ent + 8), first_dir_ent->name_len)==0)) {
            fprintf(stderr, "File or directory already exists.");
            exit(EEXIST);
        }
        k += first_dir_ent->rec_len;
        first_dir_ent = (void*)(first_dir_ent) + first_dir_ent -> rec_len;
    }
}

// TODO remember to free result
char* get_last_name(ll* ll_head){
    ll* loop = ll_head;
    while (loop->next!=NULL){
        loop=loop->next;
    }
    char* result = malloc((loop->name_len+1)* sizeof(char));
    memset(result, '\0', (loop->name_len+1));
    strncpy(result, loop->name, (size_t) loop->name_len);
    return result;
}

// TODO remember to free result
char* get_sec_last_name(ll* ll_head, int ll_length){
    ll* loop = ll_head;
    for (int i = 0; i < ll_length - 1; i++){
        loop=loop->next;
    }
    char* result = malloc((loop->name_len+1)* sizeof(char));
    memset(result, '\0', (loop->name_len+1));
    strncmp(result, loop->name, (size_t) loop->name_len);
    return result;
}