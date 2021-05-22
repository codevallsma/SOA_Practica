/*** Name:
*   Eric Vallsmadella
* Login:
*   eric.vallsmadella
*/

#include "utils/Utils.h"
#include "utils/screenOutput.h"
#include "utils/fileManagement.h"

/*
Switch that allows us to know which option the user is requesting
*/
void menu_options(char * volumeName, char * option, char *argv[], int argc){

  //opening the volume file and if it does not exist display error to the user
  int volumeFile = readFileName(volumeName);

  //checking if the volume file exists
  if (volumeFile != -1) {
    if (strcmp(option,"/info" )==0){
      //option info, now we have to figure out what type of volume is
      read_file_volume_info(volumeFile);
    } else if ( strcmp(option,"/find" )==0){
        if(argc == 4) {
            findFileInVolume(volumeFile, (char *) argv[3],0);
        } else {
            printaColors(BOLDRED,ARG_ERROR);
        }
      //option find
    } else if ( !strcmp(option,"/delete") ){
        if(argc == 4) {
            findFileInVolume(volumeFile, (char *) argv[3],1);
        } else {
            printaColors(BOLDRED,ARG_ERROR);
        }
      //option delete
    } else{
      //argument errors
      printaColors(BOLDRED,ERR_ARGS);
    }
    //closing file descriptor of the file
    close(volumeFile);
  }

}

int main(int argc, char *argv[]) {
  if (argc < 3) {
        printaColors(BOLDRED,ARG_ERROR);
        return 1;
  }else{
    menu_options((char*)argv[2], (char*)argv[1], argv, argc);
  }
  return 0;
}
