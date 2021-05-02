# SOA_Practica
In this repo we try to read file systems in c, to be exact we will read FAT16 and EXT2 volumnes. We divided the project in 4 phases: <br>
* **Phase 1:**<br> Info: Displays the information of FAT16 and ext2 filesystems<br><br>
* **Phase 2:**<br> Find: Given a file name by parameters the program tries to find the file inside the filesystem and if it is found, displays the size in bytes, if not, it displays an error message.<br><br>
  
In order to execute the program run make on the FASE_X folder and execute the command:<br>**./shooter \<operacio\> \<nom_volum\> \<nom_arxiu\>**<br>
<br>The parameters are used as following:

| PARAMETERS   |      PHASE 1      |   PHASE 2 |
|----------|:-------------:|:------:|
| \<operacio\> | /info | /find |
| \<nom_volum\> |    The volume name that we want to read   |   Same as Phase 1 |
| \<nom_arxiu\> | Not available for phase 1 |    File name to be seeked inside the volume |
<br>

   

