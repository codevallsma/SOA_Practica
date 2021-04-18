#include "screenOutput.h"

/**
 * @brief  Prints through screen number of files
 * 
 * @param  numFiles: number of files
 * 
 */
void printaNumFiles(int numFiles) {
  char* buffer;
  int size = asprintf(&buffer,FILES_FOUND, numFiles);
  write(1, buffer, size);
  free(buffer);
}

/**
 * @brief Prints message with a specific color
 * 
 * @param  frase: message that will be printed
 * 
 */
void printaColors(char* color, char* frase){
  char* buffer;
  int size = asprintf(&buffer,"%s%s%s%s", color, frase,"\n", RESET);
  write(1, buffer, size);
  free(buffer);
}

/**
 * @brief  Prints float with a specific color
 *    
 * @param  frase: float to be printed
 * 
 */
void printaColorsFloat(char*color, float frase){
  char* buffer;
  int size = asprintf(&buffer,"%s%.1f%s%s", color, frase,"\n", RESET);
  write(1, buffer, size);
  free(buffer);
}

/**
 * @brief  Prints message that the file doesn't exist in red
 * 
 * @param  filename: file name
 * 
 */
void errorSout(char* filename){
  char* buffer;
  asprintf(&buffer,"%s%s%s", "File ", filename," doesn't exist");
  printaColors(BOLDRED, buffer);
  free(buffer);
}

/**
 * @brief  Prints number in a specific color
 * 
 * @param  color : message color
 * @param  num: number to be printed
 * 
 */
void printItoa(char* color, int num){
  char* buffer;
  buffer = itoaCustom(num,10);
  int size = asprintf(&buffer,"%s%s%s%s", color, buffer,"\n", RESET);
  write(1, buffer, size);
  free(buffer);
}

/**
 * @brief  Shows files through through screen
 * 
 * @param  numFiles: number of files
 * @param  files: char array with all the files
 * 
 */
void showFiles(int numFiles, char** files) {
  for (int i=0; i<numFiles; i++) {
    write(1,files[i],strlen(files[i]));
    write(1,"\n",1);
  }
  write(1,"\n",1);
}

/**
 * @brief  Prints directory not found message in red
 * 
 * @param  path: directory path
 * 
 */
void printNoDirectory(const char* path) {
  char* buffer;
  asprintf(&buffer,"%s%s", NO_DIRECTORY_FOUND, path);
  printaColors(BOLDRED, buffer);
  free(buffer);
}

