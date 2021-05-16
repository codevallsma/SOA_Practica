# SOA_Practica
In this repo we try to read file systems in c, to be exact we will read FAT16 and EXT2 volumnes. We divided the project in 4 phases: <br>
* **Phase 1:**<br> Info: Displays the information of FAT16 and ext2 filesystems<br><br>
* **Phase 2:**<br> Find: Given a file name by parameters the program tries to find the file inside the filesystem root directory and if it is found, displays the size in bytes, if not, it displays an error message.<br><br>
* **Phase 3:**<br> Find: Given a file name by parameters the program tries to find the file inside all the filesystem and if it is found, displays the size in bytes, if not, it displays an error message. Since fat files can only have 8 bytes,  we only compare the first 6 characters to compare the file if the filename is larger than 8 characters<br><br>
For example, if we want to find libssl.so.0.9.8<br><br>
![](documentation/FAT_STRUCTURE.png "Title")
  <br><br>We will have to type: <br><br>
![](documentation/libsslSearch.png "Title")
  
In order to execute the program run make on the FASE_X folder and execute the command:<br>**./shooter \<operacio\> \<nom_volum\> \<nom_arxiu\>**<br>
<br>The parameters are used as following:

| PARAMETERS   |      PHASE 1      |   PHASE 2 |   PHASE 3 |
|----------|:-------------:|:------:| ------:|
| \<operacio\> | /info | /find | /find|
| \<nom_volum\> |    The volume name that we want to read   |   Same as Phase 1 |Same as Phase 1|
| \<nom_arxiu\> | Not available for phase 1 |    File name to be seeked inside the volume |  File name to be seeked inside the volume|
<br>

   

