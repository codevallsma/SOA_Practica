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
/******************************************************** DELETE *************************************************************/
u_char fileToDeleteExt2;
/******************************************************** FIND *************************************************************/
//global variables
unsigned int inode_table;
unsigned int s_inodes_per_group;
unsigned int s_blocks_per_group;
uint num_of_subdirs =0;
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
    //creating variabke inode
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
    //comparing if the filenames are the same
    if (!strcmp(userFile, entry.name)) {
        //getting the inode of the filename found
        struct Ext2_inode nextInode = getInode(volume, entry.inode, block_size);
        //printing size in bytes
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
    //we have to read the directoryPath length
    read(volumeFd, &entry.rec_len, 2);
    //we also have to read the name length
    read(volumeFd, &entry.name_len, 1);
    //reading the file type
    read(volumeFd, &entry.file_type, 1);
    //filling the array with \0
    memset(entry.name,'\0',EXT2_NAME_LENGTH);
    //finally reading the file name
    readUntilStatic(volumeFd,entry.name, entry.name_len+1, 32);
    //returning the entry value
    return entry;
}

void incrementFreeInodes(int fd, unsigned int block_size){
    unsigned short int bg_free_inodes_count;
    // Read size of inodes
    lseek(fd, OFFSET_FREE_INODES_COUNT + SUPERBLOCK_OFFSET + block_size, SEEK_SET);
    read(fd, &bg_free_inodes_count, sizeof(bg_free_inodes_count));
    //increment the size of the free inodes
    bg_free_inodes_count++;
    //write the size changes in disk
    lseek(fd, OFFSET_FREE_INODES_COUNT + SUPERBLOCK_OFFSET + block_size, SEEK_SET);
    write(fd, &bg_free_inodes_count, sizeof(bg_free_inodes_count));
}

void skipCurrentEntry(int fdVolume, unsigned char block[], unsigned int blockOffset, unsigned int lastSize, __u16 currentEntryRec_len){
    struct Ext2_dir_entry *firstEntry, *lastEntry;
    firstEntry = (struct Ext2_dir_entry *)block;
    // Get the last entry and add the rec_len of the actual entry
    lastEntry = (void *)firstEntry + lastSize;
    //we are doing this to skip the entry we want to delete
    lastEntry->rec_len = lastEntry->rec_len + currentEntryRec_len;
    // Write the new rec_len
    lseek(fdVolume, blockOffset + lastSize + 4, SEEK_SET);
    //write changes to disk
    write(fdVolume, &(lastEntry->rec_len), 2);
}

void removeEntry(int fdVolume, struct Ext2_dir_entry *entry, unsigned int block_size,
                 unsigned int lastSize, struct Ext2_inode inode)
{
    unsigned char bitmap, new_bitmap, block[block_size];
    unsigned int bg_inode_bitmap;
    unsigned int blockOffset = getBlockOffset(inode.i_block[0], block_size);

    // Read the first inode's entry
    lseek(fdVolume, blockOffset , SEEK_SET);
    read(fdVolume, block, block_size);

    //increment the rec_len size of the last directory
    skipCurrentEntry(fdVolume, block, blockOffset, lastSize, entry->rec_len);
    //incrementing the size of free inodes
    incrementFreeInodes(fdVolume, block_size);

    // Read block group inode bitmap
    lseek(fdVolume,  SUPERBLOCK_OFFSET + block_size + 4, SEEK_SET);
    read(fdVolume, &bg_inode_bitmap, sizeof(bg_inode_bitmap));

    // Read the bitmap where the removed entry is
    uint increment = entry->inode / 8;

    lseek(fdVolume, bg_inode_bitmap*block_size + increment, SEEK_SET);
    read(fdVolume, &bitmap, sizeof(bitmap));

    // Set the bit on the bitmap to 0
    unsigned char mask = ~(0x80 >> (entry->inode - 8 * increment));
    new_bitmap = bitmap & mask;

    // Write the updated bitmaps
    lseek(fdVolume, bg_inode_bitmap*block_size + increment, SEEK_SET);
    write(fdVolume, &new_bitmap, sizeof(new_bitmap));
}

void manageDeleteOrFind(int fd, struct Ext2_inode inode, unsigned int block_size, struct Ext2_dir_entry *dirEntry,  uint32_t last_size){
    //checking if the file has to be deleted or not
    if(fileToDeleteExt2) {
        //the file we found has to be deleted
        removeEntry(fd, dirEntry, block_size, last_size, inode);
    } else {
        //we have to display the size of the file in bytes
        //getting the inode of the filename found
        struct Ext2_inode nextInode = getInode(fd , dirEntry->inode, block_size);
        //printing size in bytes
        printf("%s %u bytes.\n\n", FILE_FOUND, nextInode.i_size);
    }
}

/**
This function looks for the fileName given by parameters and returns true if it exists in the root directoryPath of ext2
*/
bool lookForFile(int fd, struct Ext2_inode inode, char *fileName, unsigned int block_size, char*** paths) {
    struct Ext2_dir_entry dirEntry;
    uint32_t size;
    uint32_t last_size;
    uint32_t dir_offset;
    //we have to read each directoryPath entry of the root Directory
    for (int index_pointers = 0; index_pointers < 12; index_pointers++) {
        //cheking if the pointer is different than 0
        if (inode.i_block[index_pointers] != 0) {
            dir_offset = getBlockOffset(inode.i_block[index_pointers], block_size);
            size = 0;
            //getting the directoryPath entry
            dirEntry = getEntry(fd, dir_offset);
            //looping inside the data block to find all the directories
            while (size < inode.i_size) {
                //cheking if the entry is a file or a directoryPath
                if (EXT2_FT_REG_FILE == dirEntry.file_type) {
                    //checking if the filename is the same as the one we are looking for
                    if (!strcmp(fileName, dirEntry.name)){
                        //do the things we need to do when finding the file, either delete it or find it's size in bytes
                        manageDeleteOrFind(fd, inode, block_size, &dirEntry, last_size);
                        return true;
                    }
                //checking if the file we found is a directoryPath
                } else if (EXT2_FT_DIR == dirEntry.file_type) {
                    //searchDirectory
                    if (dirEntry.name[0] != '.') {
                        //getting the inode for the corresponding dir entry
                        struct Ext2_inode inodeAux = getInode(fd, dirEntry.inode, block_size);
                        //doing a recursive call to examine the next inode block
                        if (lookForFile(fd, inodeAux, fileName, block_size, paths)) {
                            //copying subidirs
                            *paths = copyDirPaths(dirEntry.name, *paths, &num_of_subdirs);
                            //returning true if the file was found
                            return true;
                        }

                    }
                }
                //the rec_len is the size of the current directoryPath so for the next dir, we have to move rec_len bytes to start reading
                dir_offset += dirEntry.rec_len;
                last_size = size;
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
void find_Ext2(int fd, char *fileName, u_char fileToDeleteExt2Func) {
    int block_size;
    char** paths;

    //storing the mode in which we are in, deleting a file or searching one
    fileToDeleteExt2 = fileToDeleteExt2Func;
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

    //number of subdirectories
    num_of_subdirs = 0;

    //check if the inode is a directoryPath
    if (S_ISDIR(inode.i_mode)) {
        //now we have to look for the file inside the root directoryPath
        if(!lookForFile(fd, inode, fileName, block_size, &paths)){
            //if we have not found the file print an error message
            printaColors(BOLDRED,EXT_FILE_NOT_FOUND);
        } else {
            //if we have to delete the file we have to print a message of success deleting the file
            if(fileToDeleteExt2){
                printaColors(BOLDGREEN, EXT_FILE_DELETED);
            }
            printPaths(paths,fileName,num_of_subdirs);
            freePath(paths, num_of_subdirs);
        }
    }
}
