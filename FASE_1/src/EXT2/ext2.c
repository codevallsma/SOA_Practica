#include "ext2.h"

/**
* Returns 1 if the file is EXT2
*/
int isEXT2(int fd){
  short type;
  lseek(fd,SUPERBLOCK_OFFSET + 56, SEEK_SET);
  read(fd, &type, sizeof(type));
	return type == -4269;
}

void showExt2InodeInfo(int fd){
  short twoBytes;
  int fourBytes;

  // Info node display in green
  printaColors(BOLDGREEN,INFO_INODE);

  // SIZE INODE
  lseek(fd,SUPERBLOCK_OFFSET + 88, SEEK_SET);
  read(fd,&twoBytes, sizeof(twoBytes));
  printf("%s %d\n", SIZE_INODE, twoBytes);

  // NUM INODE
  lseek(fd,SUPERBLOCK_OFFSET, SEEK_SET);
  read(fd,&fourBytes, sizeof(fourBytes));
  printf("%s %d\n", NUM_INODE, fourBytes);

  // FIRST INODE
  lseek(fd,SUPERBLOCK_OFFSET + 84, SEEK_SET);
  read(fd,&fourBytes, sizeof(fourBytes));
  printf("%s %d\n", FIRST_INODE, fourBytes);

  // GROUP INODE
  lseek(fd,SUPERBLOCK_OFFSET + 40, SEEK_SET);
  read(fd,&fourBytes, sizeof(fourBytes));
  printf("%s %d\n", GROUP_INODE, fourBytes);

  // FREE INODE
  lseek(fd,SUPERBLOCK_OFFSET + 16, SEEK_SET);
  read(fd,&fourBytes, sizeof(fourBytes));
  printf("%s %d\n", FREE_INODE, fourBytes);
}

void showEXT2_BlockInfo(int fd){

	int fourBytes;

  printaColors(BOLDGREEN,INFO_BLOCKS);
	// SIZE BLOCK
  lseek(fd, SUPERBLOCK_OFFSET + 24, SEEK_SET);
	read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", SIZE_BLOCKS, SUPERBLOCK_OFFSET << fourBytes);

	// RESERVED BLOCKS
  lseek(fd, SUPERBLOCK_OFFSET + 8, SEEK_SET);
	read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", RESERVED_BLOCKS, fourBytes);

	// FREE BLOCKS
  lseek(fd, SUPERBLOCK_OFFSET + 12, SEEK_SET);
	read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", FREE_BLOCKS, fourBytes);

	// TOTAL BLOCKS
  lseek(fd, SUPERBLOCK_OFFSET + 4, SEEK_SET);
	read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", TOTAL_BLOCKS, fourBytes);

	// FIRST BLOCK
  lseek(fd, SUPERBLOCK_OFFSET + 20, SEEK_SET);
	read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", FIRST_BLOCKS, fourBytes);

	// BLOCK GROUP
  lseek(fd, SUPERBLOCK_OFFSET + 32, SEEK_SET);
  read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", GROUP_BLOCKS, fourBytes);

	// FRAGS BLOCK
  lseek(fd, SUPERBLOCK_OFFSET + 36, SEEK_SET);
	read(fd,&fourBytes, sizeof(fourBytes));
	printf("%s %d\n", FRAGS_BLOCKS, fourBytes);

}

void showEXT2info(int fd) {
  //display inode information
  printf("\n\n");
  showExt2InodeInfo(fd);
  showEXT2_BlockInfo(fd);
}
