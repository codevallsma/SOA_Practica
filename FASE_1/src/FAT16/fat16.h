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
#define OFFSET_FAT_SECTOR 22
#define OFFSET_LABEL 43

#include "../utils/generalLibreries.h"
#include "../utils/screenOutput.h"

int isFAT16(int fd);
void showFAT16info(int fd);

#endif
