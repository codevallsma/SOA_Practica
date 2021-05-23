# SOA_Practica
## REQUIREMENTS

In this repo we try to read file systems in c, to be exact we will read FAT16 and EXT2 volumnes. We divided the project in 4 phases: <br>
* **Phase 1:**<br> Info: Displays the information of FAT16 and ext2 filesystems<br><br>
* **Phase 2:**<br> Find: Given a file name by parameters the program tries to find the file inside the filesystem root directory and if it 
  is found, displays the size in bytes, if not, it displays an error message.<br><br>
* **Phase 3:**<br> Find: Given a file name by parameters the program tries to find the file inside all the filesystem and if it is found, 
  displays the size in bytes, if not, it displays an error message. Since fat files can only have 8 bytes,  we only compare the first 6 
  characters to compare the file if the filename is larger than 8 characters<br><br>
* **Phase 4:**<br> Delete: Given a filename by parameters, this program searches this file through the desired volume and deletes it from 
  the file system<br>

## HOW TO COMPILE THE PROJECT
In order to execute the program run make on the FASE_X folder and execute the command:<br>**./shooter \<operacio\> \<nom_volum\> \<nom_arxiu\>**<br>
<br>The parameters are used as following:

| PARAMETERS    |                 PHASE 1                |                     PHASE 2                |                 PHASE 3                  |                PHASE 4                    |
|----------     |:---------------------------------------:|:------------------------------------------:|:----------------------------------------:|: ----------------------------------------:|
| \<operacio\>  | /info                                  |  /find                                     | /find                                    | /delete                                   |
| \<nom_volum\> | The volume name that we want to read   |   Same as Phase 1                          |  Same as Phase 1                         |  Same as Phase 1                          |
| \<nom_arxiu\> | Not available for phase 1              |   File name to be seeked inside the volume |  File name to be seeked inside the volume|  File name to be deleted inside the volume|
<br>

Problems may occur when searching filenames with FAT 16 filenames, because VFAT is not implemented in this project, so the maximum file name characters is 8. 
If the filename we want to search has more than 8 characters we will have to type the following:<br><br>
For example, if we want to find libssl.so.0.9.8<br><br>
![](documentation/FAT_STRUCTURE.png "Title")
<br><br>We will have to type: <br><br>
![](documentation/libsslSearch.png "Title")

As this is a problem with filename sizes in when working with fat16 we will encounter this problem as in phase 2, 3 and 4 with fat16 filesystems.

## EXPLANATION ABOUT THE PROJECT
### 1. DESIGN (How we designed the project)
The project has been split in three different folders inside the **FASE_X/src** folder :
1. The **FAT16 folder**: <br>This is the folder where we can find the fat header and c files that contained all the functions related with 
   FAT16 filesystem managing. <br><br>
2. The **EXT2 folder**: <br>This is the folder where we can find the fat header and c files that contained all the functions related with
   EXT2 filesystem managing.<br><br>
3. The **Utils folder**: <br>This is the folder where we can find all the functions related with opening files, reading information from 
   file descriptors and displaying information on the screen.<br><br>   
   
### 2. USE OF DATA STRUCTURES
#### 2.1 Data structures for the ext2 filesystem
This is the inode struct. We try to avoid it as long as we can, because it takes up a lot of memory space in ram, but sometimes is 
very useful when we want to pass a lot of parameters related with inode information to a function.
```c
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
```
We use the following data structure when iterating through data blocks to find the file we are looking for. 
We found it useful because it provides a lot of information of the file and we use all fields. 
```c 
//We used this data structure only when iterating through directories 
struct Ext2_dir_entry {
    __u32 inode;            /* Inode number */
    __u16 rec_len;        /* Directory entry length */
    __u8 name_len;        /* Name length */
    __u8 file_type;
    char name[EXT2_NAME_LENGTH];    /* File name */
};
```

#### 2.2 Data structures for the FAT16
The following struct is used when iterating file entries trying to find the file inserted. This struct was found on [the internet](//https://codeandlife.com/2012/04/02/simple-fat-and-sd-tutorial-part-1/) 
but it had lots of fields, so we reduced the fields to the ones we are sure that we will always use.
```c
typedef struct {
    //THE +1 to include the \0
    unsigned char filename[FILENAME_SIZE +1];
    //THE +1 to include the \0
    unsigned char ext[EXTENSION_SIZE+1];
    unsigned char attributes;
    unsigned char reserved[10];
    unsigned short modify_time;
    unsigned short modify_date;
    unsigned short starting_cluster;
    uint file_size;
} __attribute((packed)) Fat16Entry;
```
The next struct is used to scan the information of the FAT16 filesystem. This struct is useful for calculating byte offsets when searching in subdirectories and printing the volume information to the screen.
```c
typedef struct {
    uint8_t systName[8];
    uint16_t sectorSize;
    uint8_t sectPerCluster;
    uint16_t BPB_ResvdSecCnt;
    uint8_t numFats;
    uint16_t maxRootEntries;
    uint16_t BPB_FATsz16;
    uint8_t volLabel[11];
    uint16_t totalSectors;
} Fat16BootSector;
```
### (d) TESTS PERFORMED.
We used