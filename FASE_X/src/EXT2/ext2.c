#include "ext2.h"
/******************************************************** INFO *************************************************************/
/**
* Returns 1 if the file is EXT2
*/
int isEXT2(int fd) {
    short type;
    lseek(fd, SUPERBLOCK_OFFSET + 56, SEEK_SET);
    read(fd, &type, sizeof(type));
    return type == -4269;
}

/**
* Read and print to the screen node information
*/
void showExt2InodeInfo(int fd) {
    short twoBytes;
    int fourBytes;

    // Info node display in green
    printaColors(BOLDGREEN, INFO_INODE);

    // SIZE INODE
    lseek(fd, SUPERBLOCK_OFFSET + 88, SEEK_SET);
    read(fd, &twoBytes, sizeof(twoBytes));
    printf("%s %d\n", SIZE_INODE, twoBytes);

    // NUM INODE
    lseek(fd, SUPERBLOCK_OFFSET, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", NUM_INODE, fourBytes);

    // FIRST INODE
    lseek(fd, SUPERBLOCK_OFFSET + 84, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", FIRST_INODE, fourBytes);

    // GROUP INODE
    lseek(fd, SUPERBLOCK_OFFSET + 40, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", GROUP_INODE, fourBytes);

    // FREE INODE
    lseek(fd, SUPERBLOCK_OFFSET + 16, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", FREE_INODE, fourBytes);
}

/**
* Read and print to the screen block information
*/
void showEXT2_BlockInfo(int fd) {

    int fourBytes;

    printaColors(BOLDGREEN, INFO_BLOCKS);
    // SIZE BLOCK
    lseek(fd, SUPERBLOCK_OFFSET + 24, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", SIZE_BLOCKS, SUPERBLOCK_OFFSET << fourBytes);

    // RESERVED BLOCKS
    lseek(fd, SUPERBLOCK_OFFSET + 8, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", RESERVED_BLOCKS, fourBytes);

    // FREE BLOCKS
    lseek(fd, SUPERBLOCK_OFFSET + 12, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", FREE_BLOCKS, fourBytes);

    // TOTAL BLOCKS
    lseek(fd, SUPERBLOCK_OFFSET + 4, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", TOTAL_BLOCKS, fourBytes);

    // FIRST BLOCK
    lseek(fd, SUPERBLOCK_OFFSET + 20, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", FIRST_BLOCKS, fourBytes);

    // BLOCK GROUP
    lseek(fd, SUPERBLOCK_OFFSET + 32, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", GROUP_BLOCKS, fourBytes);

    // FRAGS BLOCK
    lseek(fd, SUPERBLOCK_OFFSET + 36, SEEK_SET);
    read(fd, &fourBytes, sizeof(fourBytes));
    printf("%s %d\n", FRAGS_BLOCKS, fourBytes);

}

/**
* Read and print to the screen volume's information
*/
void showEXT2_VolumeInfo(int fd) {

    time_t time;

    printaColors(BOLDGREEN, INFO_VOLUME);
    // VOLUME NAME
    char name[16];
    lseek(fd, SUPERBLOCK_OFFSET + 120, SEEK_SET);
    read(fd, name, sizeof(name));
    printf("%s %s\n", VOLUME_NAME, name);

    // LAST CHECK
    lseek(fd, SUPERBLOCK_OFFSET + 64, SEEK_SET);
    read(fd, &time, sizeof(time));
    printf("%s %s", LAST_CHECK, asctime(gmtime(&time)));

    // LAST MOUNTING
    lseek(fd, SUPERBLOCK_OFFSET + 52, SEEK_SET);
    read(fd, &time, 2);
    printf("%s %s", LAST_MOUNTING, asctime(gmtime(&time)));

    // LAST WRITING
    lseek(fd, SUPERBLOCK_OFFSET + 48, SEEK_SET);
    read(fd, &time, 2);
    printf("%s %s\n", LAST_WRITING, asctime(gmtime(&time)));

}

void showEXT2info(int fd) {
    //display inode information
    printf("\n\tFilesystem: EXT2\n");
    printf("\n\n");
    showExt2InodeInfo(fd);
    showEXT2_BlockInfo(fd);
    showEXT2_VolumeInfo(fd);
    printf("\n\n");
}

/******************************************************** FIND *************************************************************/
//global variables
unsigned int inode_table;
unsigned int s_inodes_per_group;
unsigned int s_blocks_per_group;

/**
Given a block size and the block number we can work out the position of the next block
*/
unsigned int getBlockOffset(unsigned int block, unsigned int block_size) {
    return SUPERBLOCK_OFFSET + (block - 1) * block_size;
}

/**
Function that reads the correspondant inode given the inode number
*/
struct Ext2_inode getInode(int fd, unsigned int inode_num, unsigned int block_size) {
    //we have to know the group of the inode
    //block group, page 22 pdf ext2
    unsigned int block_group = (inode_num - 1) / s_inodes_per_group;
    //Once the block is identified, the local inode index for the local inode table can be identified using:
    unsigned int local_inode_index = (inode_num - 1) % s_inodes_per_group;
    //asking for dynamic memory
    struct Ext2_inode inode;
    //calculating the offset for the group
    unsigned int groupPosition = block_group * s_blocks_per_group * block_size;
    //postioning the pointer of the data that we want to read to
    lseek(fd, getBlockOffset(inode_table, block_size) + groupPosition + local_inode_index * sizeof(struct Ext2_inode),
          SEEK_SET);
    //reading the inode content
    read(fd, &inode, sizeof(struct Ext2_inode));
    //returning the pointer of the inode struct
    return inode;
}

/**
* Read file name and compare if it's the same name as the user's input
*/
int checkFile(int volume, char *userFile, struct Ext2_dir_entry entry, unsigned int block_size) {

    // Read directoy's name
    char file_name[EXT2_NAME_LENGTH + 1];
    memcpy(file_name, entry.name, entry.name_len);
    file_name[entry.name_len] = '\0';              /* append null char to the file name */

    if (!strcmp(userFile, file_name)) {
        struct Ext2_inode nextInode = getInode(volume, entry.inode, block_size);
        printf("%s %u bytes.\n\n", FILE_FOUND, nextInode.i_size);
        return true;
    }

    return false;
}
/**
 * Returns an entry of the file system given the offset in bytes
 * @param volumeFd : the file descriptor of the volume
 * @param offset
 * @return : The entry struct read
 */
struct Ext2_dir_entry getEntry(int volumeFd, unsigned int offset) {
    struct Ext2_dir_entry entry;
    //reading the inode first
    lseek(volumeFd, offset, SEEK_SET);
    read(volumeFd, &entry.inode, 4);
    //we have to read the directory length
    read(volumeFd, &entry.rec_len, 2);
    //we also have to read the name length
    read(volumeFd, &entry.name_len, 1);
    //reading the file type
    read(volumeFd, &entry.file_type, 1);
    //filling the array with \0
    memset(entry.name,'\0',EXT2_NAME_LENGTH);
    //finally reading the file name
    read(volumeFd, entry.name, entry.name_len);

    //returning the entry value
    return entry;
}

/**
This function looks for the fileName given by parameters and returns true if it exists in the root directory of ext2
*/
bool lookForFile(int fd, struct Ext2_inode inode, char *fileName, unsigned int block_size) {
    struct Ext2_dir_entry dirEntry;
    uint32_t size;
    uint32_t dir_offset;
    //we have to read each directory entry of the root Directory
    for (int index_pointers = 0; index_pointers < 12; index_pointers++) {
        //cheking if the pointer is different than 0
        if (inode.i_block[index_pointers] != 0) {
            dir_offset = getBlockOffset(inode.i_block[index_pointers], block_size);
            size = 0;
            //getting the directory entry
            dirEntry = getEntry(fd, dir_offset);
            //looping inside the data block to find all the directories
            while (size < inode.i_size) {
                //cheking if the entry is a file or a directory
                if (EXT2_FT_REG_FILE == dirEntry.file_type) {
                    //checking if the filename is the same as the one we are looking for
                    if (checkFile(fd, fileName, dirEntry, block_size)){
                        return true;
                    }
                //checking if the file we found is a directory
                } else if (EXT2_FT_DIR == dirEntry.file_type) {
                    //searchDirectory
                    if (dirEntry.name[0] != '.') {
                        //getting the inode for the corresponding dir entry
                        struct Ext2_inode inodeAux = getInode(fd, dirEntry.inode, block_size);
                        //doing a recursive call to examine the next inode block
                        if (lookForFile(fd, inodeAux, fileName, block_size)) {
                            //returning true if the file was found
                            return true;
                        }
                    }
                }
                //the rec_len is the size of the current directory so for the next dir, we have to move rec_len bytes to start reading
                dir_offset += dirEntry.rec_len;
                size += dirEntry.rec_len;
                //get the next entry
                dirEntry = getEntry(fd, dir_offset);
            }
        }
    }
    return false;
}


/**
This function tries to find the file given the filename inside an ext2 filesystem
*/
void find_Ext2(int fd, char *fileName) {
    int block_size;

    //first read block size from the superblock
    lseek(fd, SUPERBLOCK_OFFSET + S_LOG_BLOCK_SIZE, SEEK_SET);
    read(fd, &block_size, sizeof(int));
    //calculate block size so we can go to the block descriptor table
    block_size = SUPERBLOCK_OFFSET << block_size;

    //once in the block descriptor table we obtain the inode table
    lseek(fd, SUPERBLOCK_OFFSET + BG_INODE_TABLE + block_size, SEEK_SET);
    read(fd, &inode_table, sizeof(int));

    //we also have to read the s_inodes_per_group
    lseek(fd, SUPERBLOCK_OFFSET + EXT2_INODES_PER_GROUP_OFFSET, SEEK_SET);
    read(fd, &s_inodes_per_group, sizeof(int));

    //we also have to read the s_blocks_per_group
    lseek(fd, SUPERBLOCK_OFFSET + EXT2_BLOCKS_PER_GROUP_OFFSET, SEEK_SET);
    read(fd, &s_blocks_per_group, sizeof(int));

    // Get the root inode
    struct Ext2_inode inode = getInode(fd, ROOT_INODE, block_size);

    //check if the inode is a directory
    if (S_ISDIR(inode.i_mode)) {
        //now we have to look for the file inside the root directory
        if(!lookForFile(fd, inode, fileName, block_size)){
            //if we have not found the file print an error message
            printaColors(BOLDRED,EXT_FILE_NOT_FOUND);
        }
    }
}
