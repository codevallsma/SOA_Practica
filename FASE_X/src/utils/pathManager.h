#ifndef _PATH_MANAGER_H_
#define _PATH_MANAGER_H_

#include "../utils/generalLibreries.h"
#include "../utils/screenOutput.h"
#include "../utils/Utils.h"
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>

char**  copyDirPaths(char* dirName, char** paths, int* num_of_subdirs);
void freePath(char** paths, uint subdirs);

#endif
