#ifndef _EXT2_H_
#define _EXT2_H_

#define SUPERBLOCK_OFFSET 1024
#define EXT2 "\n\tFilesystem: EXT2\n\n"

#define INFO_INODE "\tINFO INODE\n"
#define SIZE_INODE "\tMida del Inode: "
#define NUM_INODE "\tNum Inode: "
#define FIRST_INODE "\tPrimer Inode: "
#define GROUP_INODE "\tInodes Grup: "
#define FREE_INODE "\tInodes Lliures: "

#define INFO_BLOCKS "\n\tINFO BLOCK\n"
#define SIZE_BLOCKS "\tMida BLOCK: "
#define RESERVED_BLOCKS "\tBlocs Reservats: "
#define FREE_BLOCKS "\tBlocs Lliures: "
#define TOTAL_BLOCKS "\tTotal Blocs: "
#define FIRST_BLOCKS "\tPrimer Bloc: "
#define GROUP_BLOCKS "\tBlock Grup: "
#define FRAGS_BLOCKS "\tFrags Grup: "

#define INFO_VOLUME "\n\tINFO VOLUM\n"
#define VOLUME_NAME "\tNom volum: "
#define LAST_CHECK "\tUltima comprov: "
#define LAST_MOUNTING "\tUltim muntatge: "
#define LAST_WRITING "\tUltima escriptura: "

#include "../utils/generalLibreries.h"
#include "../utils/screenOutput.h"

int isEXT2(int fd);
void showEXT2info(int fd);

#endif
