/*
 * All helper functions' implementations are here.
 * Created by LiAllen on 2018-03-19.
 */
#include "ext2_utils.h"

/*
 * This function is for initialize all necessary pointer for each operation.
 * On success return image file descriptor, o.w. exit.
 * Initialized variables are all global.
 */
void init_ptrs(char* img_file){
    int fd = open(img_file, O_RDWR);

    // error checking for open img_file
    if (fd < 0){
        perror("Error opening file.");
        exit(ENOENT);
    }
    // load the content from img file to disk. 128 blocks in total.
    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // create link to each reserved blocks.
    sb = (struct ext2_super_block *)(disk + EXT2_BLOCK_SIZE);
    gdt = (struct ext2_group_desc *)(disk + EXT2_BLOCK_SIZE * 2);
    block_bm = disk + (EXT2_BLOCK_SIZE * gdt->bg_block_bitmap);
    inode_bm = disk + (EXT2_BLOCK_SIZE * gdt->bg_inode_bitmap);
    inode_table = (struct ext2_inode *)(disk + EXT2_BLOCK_SIZE * gdt->bg_inode_table);
}

/*
 * This function is used for finding a free inode in inode bitmap.
 * If there is no free inode now, exit 1 and print error msg to stderr.
 */
int find_free_inode(){
    int idx = 0;
    // loop over inode bitmap. if byte is 1 pass, if 0, return index
    for (int byte = 0; byte < sb->s_inodes_count / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if (inode_bm[byte] & (1 << bit)){
                idx++;
            } else{
                return idx;
            }
        }
    }
    // code reaches here indicates error.
    fprintf(stderr, "No more available inode.\n");
    exit(1);
}

/*
 * This function is used for finding a free block in block bitmap.
 * If there is no free block now, exit 1 and print error msg to stderr.
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
    // code reaches here indicates error.
    fprintf(stderr, "No more available block.\n");
    exit(1);
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
    // loop over each byte
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

/*
 * Convert path to a linked list, so we can do more versatile operations.
 * All linked list nodes are malloced
 */
void construct_ll(char* path, ll** link_list){
    *link_list = NULL;

    int length = (int) strlen(path);
    int dir_length = 0;
    for(int i = length - 1; i >= 1; i--){
        // handle last trailing slash problem
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
            // put a new node
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

/*
 * Return link list's length
 */
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
 */
void validate_path(char* path){
    if (path[0]!='/'){
        fprintf(stderr, "invalid abs path");
        exit(ENOENT);
    }
}

/*
 * Return parent's first directory entry given a link_listed path.
 */
struct ext2_dir_entry* get_parent_dir_block(ll* link_list_head){
    struct ext2_inode* current = &inode_table[EXT2_ROOT_INO - 1];
    struct ext2_dir_entry * dir_entry = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE);

    ll* current_node = link_list_head;

    int ll_length = get_ll_length(current_node);

    int k = 0;
    int j=0;
    // loop to second last link list node
    for (int i = 0; i < ll_length - 1; i++){
        while (current->i_block[j]!=0){
            while (k < EXT2_BLOCK_SIZE) {
                if ((strncmp(dir_entry->name, current_node->name, (size_t) current_node->name_len) == 0) && dir_entry->file_type==EXT2_FT_DIR) {
                    current = &inode_table[dir_entry->inode - 1];
                    dir_entry = (struct ext2_dir_entry *)(disk + current->i_block[0]*EXT2_BLOCK_SIZE);
                    if (i==ll_length-2){
                        return dir_entry;
                    }
                    // reset k
                    k=0;
                    current_node=current_node->next;
                    i++;
                } else{
                    k += dir_entry->rec_len;
                    // directory updates
                    dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
                }
            }
            j++;
            k=0;
        }
    }
    if (ll_length==1){
        return dir_entry;
    }
    // code reaches here indicates error
    fprintf(stderr, "file or directory not exist\n");
    exit(ENOENT);
}

/*
 * find a file or directory or link in a folder and return it's
 * directory entry.
 */
struct ext2_dir_entry* get_dir_ent(struct ext2_dir_entry* loop_ent, char* name){
    struct ext2_inode* parent_inode = &inode_table[loop_ent->inode - 1];

    int k = 0;
    int j=0;
    // loop over each block
    while (parent_inode->i_block[j]!=0){
        while (k < EXT2_BLOCK_SIZE) {
            if ((strncmp(loop_ent->name, name, (size_t) loop_ent->name_len) == 0)) {
                return loop_ent;    // find it
            }
            k += loop_ent->rec_len;
            loop_ent = (void*)(loop_ent) + loop_ent -> rec_len;
        }
        j++;
        k=0;
    }
    // code reaches here indicates error
    fprintf(stderr, "file or directory not exist\n");
    exit(ENOENT);
}

/*
 * Add a new directory entry to parent folder.
 * with name and type (DIR/REG/LINK)
 * Return newly added entry.
 */
struct ext2_dir_entry* add_parent_block(struct ext2_dir_entry* dir_entry, char* name, int type){
    struct ext2_inode* inode = &inode_table[dir_entry->inode-1];
    struct ext2_dir_entry* new_dir;

    // find last block.
    int k =0;
    while (k < EXT2_BLOCK_SIZE) {
        if (k+dir_entry->rec_len == EXT2_BLOCK_SIZE){
            int begin = k;
            int row_len = 8 + dir_entry->name_len;
            k+=row_len;
            // make it is divisible by 4
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
    // find last dir entry now
    if (last_padding < (strlen(name)+8)){
        // no enough space in this block
        inode->i_blocks += 2;
        inode->i_size+=EXT2_BLOCK_SIZE;
        int free_block_idx = find_free_block() + 1;
        int block_used=inode->i_blocks/2;
        inode->i_block[block_used-1]= (unsigned int) free_block_idx;
        set_bitmap(0, free_block_idx, 1);
        sb->s_free_blocks_count--;
        gdt->bg_free_inodes_count--;
        new_dir = (struct ext2_dir_entry *)(disk + inode->i_block[1]*EXT2_BLOCK_SIZE);
        k=0;
    } else{
        new_dir = (struct ext2_dir_entry *)(disk + inode->i_block[0]*EXT2_BLOCK_SIZE+k);
    }
    // assign name and other attr
    new_dir->name_len= (unsigned char) strlen(name);
    strncpy(new_dir->name, name, strlen(name));
    new_dir->rec_len= (unsigned short) (EXT2_BLOCK_SIZE - k);
    new_dir->file_type= (unsigned char) type;
    return new_dir;
}

/* Given the first directory entry and construct a linked list which
 * contains a directory entry and a pointer to next directory entry.
*/
void constrcut_dir_ll(struct ext2_dir_entry* dir_entry){
    struct ext2_inode* inode = &inode_table[dir_entry->inode-1];

    dir_ll_head=NULL;
    dir_ll* cur_dir_ll; // loop invariant
    int k =0;
    // put every new node to head and link it to original head.
    for (int i=0; i < inode->i_blocks/2;i++){
        k=0;
        dir_entry= (struct ext2_dir_entry *) (disk + EXT2_BLOCK_SIZE * inode->i_block[i]);
        while (k < EXT2_BLOCK_SIZE) {
            cur_dir_ll =malloc(sizeof(dir_ll));
            cur_dir_ll->dir_ent=dir_entry;
            cur_dir_ll->next=dir_ll_head;
            dir_ll_head = cur_dir_ll;
            // indication if it is on new block
            if (k==0){
                cur_dir_ll->new_block=1;
            } else{
                cur_dir_ll->new_block=0;
            }
            k += dir_entry->rec_len;
            dir_entry = (void*)(dir_entry) + dir_entry -> rec_len;
        }
    }

    // now reverse this linked list
    dir_ll* prev = NULL;
    dir_ll* cur = dir_ll_head;
    dir_ll* next = NULL;

    while (cur!=NULL){
        next=cur->next;
        cur->next=prev;
        prev=cur;
        cur=next;
    }
    dir_ll_head=prev;
}

void init_inode(struct ext2_inode* new_inode){
    // the following are trivial staff
    // by ext2.h we should init them to 0
    new_inode->i_gid = 0;
    new_inode->i_uid = 0;
    new_inode->osd1 = 0;
    new_inode->i_generation = 0;
    new_inode->i_file_acl = 0;
    new_inode->i_dir_acl = 0;
    new_inode->i_faddr = 0;
    new_inode->i_dtime=0;
    new_inode->i_ctime= (unsigned int) time(NULL);
    // other necessary attributes
    new_inode->i_blocks=0;
    new_inode->i_size=0;
    new_inode->i_links_count=0;
    for(int i =0;i<15;i++){
        new_inode->i_block[i]=0;
    }
}


/*
 * Given parent directory entry and name, we determine its type.
 */
int check_type(struct ext2_dir_entry* first_dir_ent , char* name){
    int k = 0;
    // find the inode in inode table
    struct ext2_inode* inode = &inode_table[first_dir_ent->inode-1];
    int has_reg = 0;
    int has_dir = 0;
    int has_link = 0;

    // find a file in a folder. since no duplicate name, only 1 can be
    // found.
    for (int i=0; i < (inode->i_blocks/2); i++) {
        while (k < EXT2_BLOCK_SIZE) {
            if ((strncmp(name, first_dir_ent->name, first_dir_ent->name_len) == 0)) {
                if (first_dir_ent->file_type==EXT2_FT_DIR){
                    has_dir+=2;
                } else if (first_dir_ent->file_type==EXT2_FT_REG_FILE){
                    has_reg+=1;
                } else if (first_dir_ent->file_type==EXT2_FT_SYMLINK){
                    has_link+=3;
                }
                break;
            }
            k += first_dir_ent->rec_len;
            first_dir_ent = (void *) (first_dir_ent) + first_dir_ent->rec_len;
        }
    }
    // returned result can be only 0 1 2 3.
    return has_dir+has_reg+has_link;
}

/*
 * return the last file/directory name in a name linked list.
 */
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

/*
 * compare entry type and inode type, if they are both the same type,
 *  return 1 ow return 0.
 */
int compare(int entry_type, int inode_type){
    if (entry_type != EXT2_FT_REG_FILE && (inode_type & EXT2_S_IFREG)){
        return 1;
    } else if (entry_type != EXT2_FT_DIR && (inode_type & EXT2_S_IFDIR)){
        return 1;
    } else if (entry_type != EXT2_FT_SYMLINK && (inode_type & EXT2_S_IFLNK)){
        return 1;
    }
    return 0;
}

/*
 * check every file in a directory if their file_type is equal to
 * imode in inode table. If there's an error, correct it, and return
 * the total number of change.
 */
int check_files_in_dir(int inode_idx){
    struct ext2_dir_entry *current = (struct ext2_dir_entry *)(disk + inode_table[inode_idx].i_block[0] * EXT2_BLOCK_SIZE);

    constrcut_dir_ll(current);

    int errors = 0;
    dir_ll* current_node = dir_ll_head;
    while (current_node != NULL) {
        if (compare(current_node->dir_ent->file_type, inode_table[current_node->dir_ent->inode-1].i_mode)){
            // if compare return 1, then there's different.
            if (inode_table[current_node->dir_ent->inode - 1].i_mode & EXT2_S_IFREG) {
                current_node->dir_ent->file_type = EXT2_FT_REG_FILE;
            } else if (inode_table[current_node->dir_ent->inode - 1].i_mode & EXT2_S_IFDIR){
                current_node->dir_ent->file_type = EXT2_FT_DIR;
                // add used dir count
                gdt->bg_used_dirs_count++;
            } else if  (inode_table[current_node->dir_ent->inode - 1].i_mode & EXT2_S_IFLNK){
                current_node->dir_ent->file_type = EXT2_FT_SYMLINK;
            }
            // print out fixed contents
            printf("Fixed: Entry type vs inode mismatch: inode [%d]\n", current_node->dir_ent->inode);
            errors += 1;
        }
        current_node = current_node->next;
    }
    return errors;
}

/*
 * the getter function is very similar to setter but it
 * returns the mode at the give idx. return -1 for error
 */
int get_bitmap(int bm_idx, int idx){
    // make idx starts from 0
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
    // loop over each bit
    for (int byte = 0; byte < num_bit / 8; byte++){
        for (int bit = 0; bit < 8; bit++){
            if ((byte*8+bit) == idx){
                if ((target_bm[byte] & (1 << (bit)))>0){
                    return 1;
                }
                return 0;
            }
        }
    }
    // indicating error
    return -1;
}

/*
 * check if each block using by inode marked as in use or
 * not in block bitmap. If block bitmap's value is not match
 * with blocks in inode, trust the blocks and print fix message.
 */
int check_blocks(int inode_idx){
    int errors = 0;
    struct ext2_inode* inode = &inode_table[inode_idx];
    int block_need = inode->i_blocks/2;

    // check commen blocks
    for (int i =0; i < block_need && i<12;i++){
        if (get_bitmap(1, inode->i_block[i]) == 0){
            set_bitmap(1, inode->i_block[i], 1);
            sb->s_free_blocks_count--;
            gdt->bg_free_blocks_count--;
            errors ++;
        }
    }
    // check indirect blocks
    if (block_need>12){
        if (get_bitmap(1, inode->i_block[12]) == 0) {
            set_bitmap(1, inode->i_block[12], 1);
            sb->s_free_blocks_count--;
            gdt->bg_free_blocks_count--;
            errors ++;
        }
        // go to indirection
        int* location = (int *)(disk + inode->i_block[12]*EXT2_BLOCK_SIZE);
        for (int k = 0; k < block_need-13; k++) {
            if (get_bitmap(1, location[k]) == 0){
                set_bitmap(1, location[k], 1);
                sb->s_free_blocks_count--;
                gdt->bg_free_blocks_count--;
                errors ++;
            }
        }
    }
    // print out fixed contents
    if (errors){
        printf("Fixed: %d in-use data blocks not marked in data bitmap for inode: [%d]\n", errors, inode_idx+1);
    }
    return errors;
}

/*
 * Given the first directory entry and construct a linked list which
 * contains a directory entry and a pointer to next directory entry.
 * DIFFERENT: different with previous one, this function return a new
 * linked list instead of change the global variable.
 * NOTE this is a special case used for rm bonus!
 */
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
            // indication if it starts from a new block
            if (k==0){
                cur_dir_ll->new_block=1;
            } else{
                cur_dir_ll->new_block=0;
            }
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

/*
 * inode_idx starts from 1
 * This function recrsive delete everything underlying this inode if possible (DIR)
 * NOTE this is a special case used for rm bonus!
 */
void release_all(int inode_idx){
    struct ext2_inode* current = &inode_table[inode_idx-1];
    if (current->i_links_count>1 && (current->i_mode&EXT2_S_IFREG)){
        current->i_links_count--;
        return;
    }
    // first delete this inode itself.
    set_bitmap(0,inode_idx,0);
    sb->s_free_inodes_count++;
    gdt->bg_free_inodes_count++;

    // minus used dir counter
    if (current->i_mode & EXT2_S_IFDIR){
        gdt->bg_used_dirs_count--;
    }
    int block_used = current->i_blocks/2;
    for (int i=0;i<block_used&&i<12;i++){
        set_bitmap(1,current->i_block[i],0);
        sb->s_free_blocks_count++;
        gdt->bg_free_blocks_count++;
    }
    // consider indirection
    if (block_used>12){
        int* indirection = (void *)(disk + current->i_block[12]*EXT2_BLOCK_SIZE);
        set_bitmap(1,current->i_block[12],0);
        sb->s_free_blocks_count++;
        gdt->bg_free_blocks_count++;
        for (int i=0; i< block_used-13;i++){
            set_bitmap(1,indirection[i],0);
            sb->s_free_blocks_count++;
            gdt->bg_free_blocks_count++;
        }
    }

    // then delete everything under this folder
    current->i_size=0;
    if (current->i_mode & EXT2_S_IFDIR){
        dir_ll* head= NULL;
        struct ext2_dir_entry* dir_ent = (struct ext2_dir_entry*) (disk + EXT2_BLOCK_SIZE*current->i_block[0]);
        head = constrcut_dir_ll_spe(dir_ent, head);

        while(head!=NULL){
            if ((strncmp(head->dir_ent->name, ".", 1)==0) || (strncmp(head->dir_ent->name, "..", 2)==0)){
                head=head->next;
            } else{
                release_all(head->dir_ent->inode);
                head=head->next;
            }
        }
        // free malloc data
        free_dir_ll(head);
    }

}

/*
 * Free link list structures used in operation.
 * this link list is for path
 */
void free_ll(ll* head){
    while (head != NULL){
        free(head->name);
        ll* temp = head->next;
        free(head);
        head = temp;
    }
}

/*
 * Free link list structures used in operation.
 * this link list is for dir ent
 */
void free_dir_ll(dir_ll* head){
    while (head != NULL){
        dir_ll* temp = head->next;
        free(head);
        head = temp;
    }
}

/*
 * Given a directory entry, return its minimal possible record length.
 */
int get_rec_len(struct ext2_dir_entry* dir_ent){
    int result = 8;
    result+=dir_ent->name_len;
    if (dir_ent->name_len==0){
        return 0;
    }
    while (result%4!=0){
        result++;
    }
    return result;
}