#include <stdbool.h>
#include "fat16.h"

/*********************************************** INFO ****************************************************/

/**
* Returns 1 if the file is FAT16
*/
int isFAT16(int fd) {
    //Checking if filesystem is FAT16
    char system_name[8];
    lseek(fd, 54, SEEK_SET);
    read(fd, system_name, sizeof(system_name));

    // Return true if the string System contains "FAT16"
    return strstr(system_name, "FAT16") != NULL;
}

/*
* Shows all the useful information from the FAT16 filesystem
*/
void showFAT16info(int fd) {
    printf(FAT16);

    short twoBytes;
    char oneByte;

    // SYSTEM NAME
    char name[8];
    lseek(fd, OFFSET_SYSTEM_NAME, SEEK_SET);
    read(fd, name, sizeof(name));
    printf("%s %s\n", SYSTEM_NAME, name);

    // SECTOR SIZE
    lseek(fd, OFFSET_SECTOR_SIZE, SEEK_SET);
    read(fd, &twoBytes, sizeof(twoBytes));
    printf("%s %d\n", SECTOR_SIZE, twoBytes);


    // SECTOR CLUSTER
    lseek(fd, OFFSET_SECTOR_CLUSTER, SEEK_SET);
    read(fd, &oneByte, sizeof(oneByte));
    printf("%s %d\n", SECTOR_CLUSTER, oneByte);


    // RESERVED SECTOR
    lseek(fd, OFFSET_RESERVED_SECTOR, SEEK_SET);
    read(fd, &twoBytes, sizeof(twoBytes));
    printf("%s %d\n", RESERVED_SECTOR, twoBytes);


    // FAT NUMBER
    lseek(fd, OFFSET_FAT_NUMBER, SEEK_SET);
    read(fd, &oneByte, sizeof(oneByte));
    printf("%s %d\n", FAT_NUMBER, oneByte);


    // MAX ROOT ENTRIES
    lseek(fd, OFFSET_MAX_ROOT_ENTRIES, SEEK_SET);
    read(fd, &twoBytes, sizeof(twoBytes));
    printf("%s %d\n", MAX_ROOT_ENTRIES, twoBytes);

    // FAT SECTOR
    lseek(fd, OFFSET_FAT_SECTOR, SEEK_SET);
    read(fd, &twoBytes, sizeof(twoBytes));
    printf("%s %d\n", FAT_SECTOR, twoBytes);

    // LABEL
    char label[11];
    lseek(fd, OFFSET_LABEL, SEEK_SET);
    read(fd, label, sizeof(label));
    printf("%s %s\n\n", LABEL, label);
}

/*********************************************** FIND ****************************************************/

/**
 * Reads the most important parts of the boot sector
 * @param fd : The file descriptor of the volume
 * @return A struct with all the boot sector information
 */
Fat16BootSector readFAT16BootSector(int fd) {
    Fat16BootSector bS;
    //FAT_Data data;
    lseek(fd, OFFSET_SYSTEM_NAME, SEEK_SET);
    //setting the \0 delimiter
    memset(bS.systName, '\0', sizeof (bS.systName));
    read(fd, bS.systName, 8);
    //sector size
    lseek(fd, OFFSET_SECTOR_SIZE, SEEK_SET);
    read(fd, &bS.sectorSize, 2);
    //sector per cluster
    lseek(fd, OFFSET_SECTOR_CLUSTER, SEEK_SET);
    read(fd, &bS.sectPerCluster, 1);
    //reserved sectors
    lseek(fd, OFFSET_RESERVED_SECTOR, SEEK_SET);
    read(fd, &bS.BPB_ResvdSecCnt, 2);
    //num fats
    lseek(fd, OFFSET_FAT_NUMBER, SEEK_SET);
    read(fd, &bS.numFats, 1);
    //max root entries
    lseek(fd, OFFSET_MAX_ROOT_ENTRIES, SEEK_SET);
    read(fd, &bS.maxRootEntries, 2);
    //sectors per fat
    lseek(fd, OFFSET_FAT_SECTOR, SEEK_SET);
    read(fd, &bS.BPB_FATsz16, 2);
    //volabel
    lseek(fd, OFFSET_LABEL, SEEK_SET);
    memset(bS.volLabel, '\0', sizeof (bS.volLabel));
    read(fd, bS.volLabel, 11);

    return bS;
}
/**
 * Gets the file size from the directory
 * @param volumeFd : the file descriptor of the filñe
 * @param rootDirSec: The root dir sector in bytes
 * @return the size in bytes of the file
 */
uint getFileSize(int volumeFd, int rootDirSec){
    uint size;
    lseek(volumeFd, rootDirSec + DIRECTORY_OFFSET_DIR_SIZE, SEEK_SET);
    read(volumeFd,&size, sizeof(uint));;
    return size;
}

/**
 *
 * @return
 */
bool readAndCompareIfEqual(int volume, uint rootDirOffset, Fat16Entry *entry, char* fileNameToSeek){
    u_char fullFilename[FILENAME_SIZE+EXTENSION_SIZE+2];

    //setting the pointer of the file to the beggining of the directory
    lseek(volume, rootDirOffset, SEEK_SET);
    //reading the filename
    readUntilStatic(volume,entry->filename, FILENAME_SIZE+1, 32);
    //reading the attribute of the file/directory
    lseek(volume, rootDirOffset+DIRECTORY_ATTR_OFFSET, SEEK_SET);
    read(volume, &(entry->attributes),sizeof(u_char));
    //skipping the bytes that correspond to the filename
    lseek(volume, rootDirOffset +FILENAME_SIZE, SEEK_SET);
    //reading the extension
    readUntilStatic(volume,entry->ext,EXTENSION_SIZE+1, 32);
    //reading starting cluster
    lseek(volume, rootDirOffset+ DIRECTORY_OFFSET_CLUSTER_LO, SEEK_SET);
    read(volume,&(entry->starting_cluster), 2 );
    //reading size
    lseek(volume, rootDirOffset + DIRECTORY_OFFSET_DIR_SIZE, SEEK_SET);
    read(volume,&(entry->file_size), sizeof(uint));;
    //setting filename to upper case so we can compare it ignoring cases
    toUpper(fileNameToSeek);
    //does the file have extension?
    if(strlen(entry->ext)>0){
        // it has an extension
        strcpy(fullFilename,entry->filename);
        int midaFullname = strlen(fullFilename);
        fullFilename[midaFullname++] = '.';
        fullFilename[midaFullname++] = '\0';
        strcat(fullFilename, entry->ext);
        return strcmp(fullFilename, fileNameToSeek) ==0;
    } else {
        // it does not have an extension
        return strcmp(fileNameToSeek,entry->filename) ==0;
    }
}
WORD getClusterValue(int fd, uint16_t cluster,Fat16BootSector bS) {
    uint32_t FATOffset = cluster * 1;
    uint32_t ThisFATSecNum = bS.BPB_ResvdSecCnt + (FATOffset / bS.sectorSize);
    uint32_t ThisFATEntOffset = (FATOffset % bS.sectorSize);
    uint32_t index = ThisFATSecNum * bS.sectorSize;

    lseek(fd, index, SEEK_SET);

    uint8_t secBuff[8][8];
    read(fd, secBuff, 8);

    WORD FATClusterValue = *((WORD *) &secBuff[ThisFATEntOffset]);

    return FATClusterValue;
}
/**
 *
 * @param fd
 * @param bS
 * @param fileToFind
 * @param rootDirOffset
 */
bool seekFile(int fd, char *fileToFind, uint32_t rootDirOffset, Fat16BootSector bS, unsigned int firstDataSector){
    Fat16Entry entry;
    do{
        bool equal = readAndCompareIfEqual(fd, rootDirOffset, &entry, fileToFind);
        //if the attribute tells us that it is a file, we have to inspect it to know if it has the same filename as the one given by parameters
        if(entry.attributes==ATTR_ARCHIVE){
            //reading the filename and the extension
            if(equal){
                uint filesize = getFileSize(fd, rootDirOffset);
                char *buffer;
                asprintf(&buffer, "\tFitxer trobat. Ocupa %u bytes\n\n", filesize);
                printaColors(BOLDGREEN, buffer);
                free(buffer);
                return true;
            }
        } else if (entry.attributes==ATTR_DIR && strcmp(entry.filename, ".") &&  strcmp(entry.filename, "..")){
           if(seekFile(fd, fileToFind, (unsigned  int)((entry.starting_cluster-2)*bS.sectPerCluster*bS.sectorSize) + firstDataSector, bS, firstDataSector)){
               //store file path
           }
        }
        // Get the next directory entry
        rootDirOffset = rootDirOffset+ DIRECTORY_ENTRY_SIZE;
    }while(entry.attributes!=0);
    printaColors(BOLDRED,"\tFILE NOT FOUND INSIDE THE FAT16 FILESYSTEM!\n\n");
    return false;
}

/**
 * Function to find the file inside the FAT16 volume
 * @param fd : The file descriptor of the volume
 * @param fileToFind: The char* with the name of the file
 */
void FAT_find(int fd, char *fileToFind) {
    //getting all the information from the boot sector
    Fat16BootSector bS = readFAT16BootSector(fd);
    unsigned int rootDirSec = (bS.BPB_ResvdSecCnt + (bS.numFats * bS.BPB_FATsz16)) * bS.sectorSize;
    unsigned int rootDirSectors = (unsigned int) ((bS.maxRootEntries * DIRECTORY_ENTRY_SIZE) + (bS.sectorSize-1)) / bS.sectorSize;
    unsigned int firstDataSector  = rootDirSec + rootDirSectors*bS.sectorSize;
    //seek the file to be found
    seekFile(fd,fileToFind, rootDirSec, bS, firstDataSector);
}