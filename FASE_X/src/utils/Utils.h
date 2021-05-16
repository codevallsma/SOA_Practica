#ifndef _UTILS_H_
#define _UTILS_H_
#define _GNU_SOURCE
//#include "generalLibreries.h"
//Llibreries del sistema
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>


int readUntilAtoiAndFree(int file, char charReadUntil);
char* readUntil(int fd, char end);
void readUntilStatic(int fd, u_char staticArray[], int totalSIze, char end);
char* readUntilNoMore(int fd, int * size);
char* itoaCustom(int val, int base);
float readFloat(int file, char charReadUntil);
void copyContentArray(char destinationArr[], char sourceArr[], int shortestSize, int longestSize);
void toLower(char* s);
void toUpper(char *s);
#endif
