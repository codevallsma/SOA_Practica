#include "fat16.h"

/**
* Returns 1 if the file is FAT16
*/
int isFAT16(int fd){
	//Checking if filesystem is FAT16
  char system_name[8];
  lseek(fd, 54, SEEK_SET);
  read(fd, system_name, sizeof(system_name));

  // Return true if the string System contains "FAT16"
  return strstr(system_name, "FAT16")!= NULL;
}

/*
* Shows all the usefull information from the FAT16 filesystem
*/
void showFAT16info(int fd){

}
