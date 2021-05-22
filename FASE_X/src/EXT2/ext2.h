#ifndef _EXT2_H_
#define _EXT2_H_

#define SUPERBLOCK_OFFSET 1024
#define ROOT_INODE 2
#define S_LOG_BLOCK_SIZE 24
#define BG_INODE_TABLE 8
#define EXT2_NAME_LENGTH 255
#define    EXT2_NDIR_BLOCKS    12                      /* number of direct blocks */
#define    EXT2_IND_BLOCK        EXT2_NDIR_BLOCKS        /* single indirect block   */
#define    EXT2_DIND_BLOCK        (EXT2_IND_BLOCK + 1)    /* double indirect block, one level to get to the information  */
#define    EXT2_TIND_BLOCK        (EXT2_DIND_BLOCK + 1)   /* triple indirect block, two keveks to get to the information  */
#define    EXT2_N_BLOCKS        (EXT2_TIND_BLOCK + 1)    /* 15 in total the last one points a group that points to groups that point to other groups*/
#define EXT2_INODES_PER_GROUP_OFFSET 40
#define EXT2_BLOCKS_PER_GROUP_OFFSET 32
#define EXT2_FT_REG_FILE 1
#define EXT2_FT_DIR 2
#define EXT2_NDIR_BLOCKS 12

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

#define OFFSET_FREE_INODES_COUNT 14

#include "../utils/generalLibreries.h"
#include "../utils/screenOutput.h"
#include "../utils/Utils.h"
#include "../utils/pathManager.h"
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned int __u32;


struct Ext2_inode {
    __u16 i_mode;         /* File type and access rights */
    __u16 i_uid;          /* Low 16 bits of Owner Uid */
    __u32 i_size;         /* Size in bytes */
    __u32 i_atime;        /* Access time */
    __u32 i_ctime;        /* Creation time */
    __u32 i_mtime;        /* Modification time */
    __u32 i_dtime;        /* Deletion Time */
    __u16 i_gid;          /* Low 16 bits of Group Id */
    __u16 i_links_count;  /* Links count */
    __u32 i_blocks;       /* Blocks count */
    __u32 i_flags;        /* File flags */
    __u32 i_osd1;
    __u32 i_block[EXT2_N_BLOCKS];  /* Pointers to blocks */
    __u32 i_generation;
    __u32 i_file_acl;
    __u32 i_dir_acl;
    __u32 i_faddr;
    __u32 i_osd2[3];
};

struct Ext2_dir_entry {
    __u32 inode;            /* Inode number */
    __u16 rec_len;        /* Directory entry length */
    __u8 name_len;        /* Name length */
    __u8 file_type;
    char name[EXT2_NAME_LENGTH];    /* File name */
};

//info
int isEXT2(int fd);

void showEXT2info(int fd);

//find
void find_Ext2(int fd, char *fileName, u_char fileToDeleteExt2Func);


#endif
