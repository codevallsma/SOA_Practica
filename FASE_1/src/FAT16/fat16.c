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
