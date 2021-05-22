#ifndef _SCREENOUTPUT_H_
#define _SCREENOUTPUT_H_

//Llibreries del sistema
#include "generalLibreries.h"
#include "Utils.h"

#define BOLD    "\033[1m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDRED   "\033[1m\033[31m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET     "\033[0m"
#define PROGRAM_NAME "shooter"
#define PROG_ARGUMENTS "<operation> <volumeName>"
#define ERR_ARGS    BOLDRED "Error, missing arguments.\nUsage: ./" PROGRAM_NAME " " PROG_ARGUMENTS  "\n" RESET
#define MSG_FILE_D    BOLDGREEN "Message from main %d" RESET " \n"
#define ERR_FILE_R  "Error when reading the file "
#define ARG_ERROR "\n\n\tError. Nombre de paràmetres incorrecte.\n\n"
#define NO_DIRECTORY_FOUND "Cannot open directory"
#define FILES_FOUND "%d files found\n"
#define EXT_FILE_NOT_FOUND "\tError! Fitxer no trobat al sistema de fitxers.\n"
#define EXT_FILE_DELETED "\tFile deleted successfully!\n"
#define FILE_FOUND "\tFitxer trobat. Ocupa "

void printaColors(char*color, char* frase );
void printaColorsFloat(char*color, float frase);
void errorSout(char* filename);
void printaNumFiles(int numFiles);
void printItoa(char* color, int num);
void printNoDirectory(const char* path);
void showFiles(int numFiles, char** files);
/**
 * Prints the filename path
 * @param path
 * @param num_of_subdirs
 */
void printPaths(char** path, char* filename, uint num_of_subdirs);
#endif
