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
/*********************************************** DELETE ****************************************************/
u_char fileToDelete;
/*********************************************** FIND ****************************************************/
uint num_of_subdirsFAT = 0;

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
    memset(bS.systName, '\0', sizeof(bS.systName));
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
    memset(bS.volLabel, '\0', sizeof(bS.volLabel));
    read(fd, bS.volLabel, 11);

    return bS;
}

/**
 * Reads the current directory information
 * @param volume : the file descriptor of the file system
 * @param entry : the entry struct
 * @param dirOffset : The file address to start reading the file
 */
void readDirInfo(int volume, Fat16Entry *entry, uint dirOffset) {
    //setting the pointer of the file to the beggining of the directory
    lseek(volume, dirOffset, SEEK_SET);
    //reading the filename
    readUntilStatic(volume, entry->filename, FILENAME_SIZE + 1, 32);
    //reading the attribute of the file/directory
    lseek(volume, dirOffset + DIRECTORY_ATTR_OFFSET, SEEK_SET);
    read(volume, &(entry->attributes), sizeof(u_char));
    //skipping the bytes that correspond to the filename
    lseek(volume, dirOffset + FILENAME_SIZE, SEEK_SET);
    //reading the extension
    readUntilStatic(volume, entry->ext, EXTENSION_SIZE + 1, 32);
    //reading starting cluster
    lseek(volume, dirOffset + DIRECTORY_OFFSET_CLUSTER_LO, SEEK_SET);
    read(volume, &(entry->starting_cluster), 2);
    //reading size
    lseek(volume, dirOffset + DIRECTORY_OFFSET_DIR_SIZE, SEEK_SET);
    read(volume, &(entry->file_size), sizeof(uint));
}

/**
 * Takes the filename found in the filesystem and commpares it with the one introduced by the user
 * @param entry : the information of the current file/directory
 * @param fileNameToSeek : the filename to be searched introduced by the user
 * @return
 */
bool compareFilenames(Fat16Entry *entry, char *fileNameToSeek) {
    u_char fullFilename[FILENAME_SIZE + EXTENSION_SIZE + 2];
    //check if the file contains ~ meaning the filename is larger than 8 characters
    if (strstr(entry->filename, "~") != NULL) {
        //looking for the position of the array containing ~
        for (int i = 0; i < FILENAME_SIZE + 1; i++) {
            //replacing ~ by \0
            if (entry->filename[i] == '~' && entry->filename[i + 1] == '1') entry->filename[i] = '\0';
        }
    }
    //does the file have extension?
    if (strlen(entry->ext) > 0) {
        // it has an extension
        strcpy(fullFilename, entry->filename);
        int midaFullname = strlen(fullFilename);
        fullFilename[midaFullname++] = '.';
        fullFilename[midaFullname++] = '\0';
        strcat(fullFilename, entry->ext);
        //comparing if filenames are the same including the extension
        return strcmp(fullFilename, fileNameToSeek) == 0;
    } else {
        // it does not have an extension
        //comparing if filenames are the same without the extension
        return strcmp(fileNameToSeek, entry->filename) == 0;
    }
}
u_char* manageExtension(Fat16Entry entry, u_char fullFilename[FILENAME_SIZE + EXTENSION_SIZE + 2]){
    strcpy(fullFilename, entry.filename);
    int midaFullname = strlen(fullFilename);
    fullFilename[midaFullname++] = '.';
    fullFilename[midaFullname++] = '\0';
    strcat(fullFilename, entry.ext);
    return fullFilename;
}
/**
 * Seeks for the filename desired by the user in the filesystem
 * @param fd: the file descriptor of the file system
 * @param bS: The boot sector information
 * @param fileToFind: THe filename to search given by the user
 * @param dirOffset: The address to start reading the file information
 */
bool
seekFile(int fd, char *fileToFind, uint32_t dirOffset, Fat16BootSector bS, unsigned int firstDataSector, char ***path) {
    Fat16Entry entry;
    do {
        readDirInfo(fd, &entry, dirOffset);
        //if the attribute tells us that it is a file, we have to inspect it to know if it has the same filename as the one given by parameters
        if (entry.attributes == ATTR_ARCHIVE) {
            //reading the filename and the extension
            if (compareFilenames(&entry, fileToFind)) {
                //if the variable fileToDelete is 0 means that the functionality that is it is meant to do is searching the file
                if(fileToDelete == 0){
                    //FILE FOUND
                    char *buffer;
                    asprintf(&buffer, "\tFitxer trobat. Ocupa %u bytes\n\n", entry.file_size);
                    printaColors(BOLDGREEN, buffer);
                    free(buffer);
                    return true;
                } else {
                    //DELETING THE FILE
                    // Setting first name character to 0xE5
                    unsigned char resetValue = 0xE5;
                    lseek(fd, dirOffset, SEEK_SET);
                    write(fd, &resetValue, 1);
                    printaColors(BOLDGREEN, "\tSuccesfully deleting the file");
                    return  true;
                }

            }
        } else if (entry.attributes == ATTR_DIR && strcmp(entry.filename, ".") && strcmp(entry.filename, "..")) {
            if (seekFile(fd, fileToFind,(unsigned int) ((entry.starting_cluster - 2) * bS.sectPerCluster * bS.sectorSize) +firstDataSector, bS, firstDataSector, path)) {
                //store file path
                if(strlen(entry.ext)>0){
                    //if the path has an extension we add it in the path
                    u_char fullFilename[FILENAME_SIZE + EXTENSION_SIZE + 2];
                    manageExtension(entry,fullFilename);
                    *path = copyDirPaths(fullFilename, *path, &num_of_subdirsFAT);
                }else {
                    //if the file has no extension we store the path as the filename
                    *path = copyDirPaths(entry.filename, *path, &num_of_subdirsFAT);
                }
                return true;
            }
        }
        // Get the next directory entry
        dirOffset = dirOffset + DIRECTORY_ENTRY_SIZE;
    } while (entry.attributes != 0);
    return false;
}

/**
 * Function to find the file inside the FAT16 volume
 * @param fd : The file descriptor of the volume
 * @param fileToFind: The char* with the name of the file
 */
void FAT_find(int fd, char *fileToFind, u_char fileToDeleteFat){
    fileToDelete = fileToDeleteFat;
    //getting all the information from the boot sector
    Fat16BootSector bS = readFAT16BootSector(fd);
    //calculating the root dir sector value
    unsigned int rootDirSec = (bS.BPB_ResvdSecCnt + (bS.numFats * bS.BPB_FATsz16)) * bS.sectorSize;
    unsigned int rootDirSectors =
            (unsigned int) ((bS.maxRootEntries * DIRECTORY_ENTRY_SIZE) + (bS.sectorSize - 1)) / bS.sectorSize;
    unsigned int firstDataSector = rootDirSec + rootDirSectors * bS.sectorSize;
    //setting filename to upper case so we can compare it ignoring cases
    toUpper(fileToFind);
    //declaring the array that will store the path to the file
    char **path;
    //seek the file to be found
    if (seekFile(fd, fileToFind, rootDirSec, bS, firstDataSector, &path)) {
        //file found
        printPaths(path, fileToFind, num_of_subdirsFAT);
        //freeing the dynamic memmory asked
        freePath(path, num_of_subdirsFAT);
    } else {
        //file not found
        printaColors(BOLDRED, "\tFILE NOT FOUND INSIDE THE FAT16 FILESYSTEM!\n\n");
    }
}