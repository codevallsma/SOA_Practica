#ifndef _FAT16_H_
#define _FAT16_H_

#define FAT16 "\n\tFilesystem: FAT16\n\n"
#define SYSTEM_NAME "\tSystem Name: "
#define SECTOR_SIZE "\tMida del sector: "
#define SECTOR_CLUSTER "\tSectors per cluster: "
#define RESERVED_SECTOR "\tSectors reservats: "
#define FAT_NUMBER "\tNúmero de FAT's: "
#define MAX_ROOT_ENTRIES "\tMaxRootEntries: "
#define FAT_SECTOR "\tSectors per FAT: "
#define LABEL "\tLabel: "

#define OFFSET_SYSTEM_NAME 3
#define OFFSET_SECTOR_SIZE 11
#define OFFSET_SECTOR_CLUSTER 13
#define OFFSET_RESERVED_SECTOR 14
#define OFFSET_FAT_NUMBER 16
#define OFFSET_MAX_ROOT_ENTRIES 17
#define OFFSET_TOTAL_SECTORS 19
#define OFFSET_FAT_SECTOR 22
#define OFFSET_LABEL 43

//pag 22 pdf fat
#define DIRECTORY_ENTRY_SIZE 32
#define DIRECTORY_ATTR_OFFSET 11
#define DIRECTORY_OFFSET_DIR_SIZE 28

#define FILENAME_SIZE 8
#define EXTENSION_SIZE 3
// 0X20 IS 32
#define ATTR_ARCHIVE 32
#include "../utils/generalLibreries.h"
#include "../utils/screenOutput.h"
#include <stdint.h>

//https://codeandlife.com/2012/04/02/simple-fat-and-sd-tutorial-part-1/
typedef struct {
    //THE +1 to include the \0
    unsigned char filename[FILENAME_SIZE +1];
    unsigned char ext[EXTENSION_SIZE+1];
    unsigned char attributes;
    unsigned char reserved[10];
    unsigned short modify_time;
    unsigned short modify_date;
    unsigned short starting_cluster;
    unsigned long file_size;
} __attribute((packed)) Fat16Entry;

typedef struct {
    uint8_t systName[8];
    uint16_t sectorSize;
    uint8_t sectPerCluster;
    uint16_t rsvrdSectors;
    uint8_t numFats;
    uint16_t maxRootEntries;
    uint16_t sectPerFat;
    uint8_t volLabel[11];
    uint16_t totalSectors;
} Fat16BootSector;

int isFAT16(int fd);
void showFAT16info(int fd);
void FAT_find(int fd, char *fileToFind);
#endif
