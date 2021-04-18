#ifndef _FILEMANAGEMENT_H_
#define _FILEMANAGEMENT_H_

#include "screenOutput.h"
#include "Utils.h"
#include <dirent.h>
#include "generalLibreries.h"
#include "../EXT2/ext2.h"
#include "../FAT16/fat16.h"

int readFileName(char* filename);
void read_file_volume_info(int fd);

#endif
