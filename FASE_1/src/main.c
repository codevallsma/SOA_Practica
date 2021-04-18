/*** Name:
*   Eric Vallsmadella
* Login:
*   eric.vallsmadella
*/

#define _GNU_SOURCE
#include "utils/generalLibreries.h"

#include "utils/Utils.h"
#include "utils/screenOutput.h"
#include "utils/fileManagement.h"

/*
Switch that allows us to know which option the user is requesting
*/
void menu_options(char * volumeName, char * option){

  //opening the volume file and if it does not exist display error to the user
  int fileConfig = readFileName(volumeName);

  //checking if the volume file exists
  if (fileConfig != -1) {
    if (strcmp(option,"/info" )==0){
      //option info, now we have to figure out what type of volume is

    } else if ( strcmp(option,"/find" )==0){
      //option find
    } else if ( strcmp(option,"/delete") ){
      //option delete
    } else{
      //argument errors
      printaColors(BOLDRED,ERR_ARGS);
    }
    //closing file descriptor of the file
    close(fileConfig);
  }

}

int main(int argc, char *argv[]) {
  if (argc != 3) {
        printaColors(BOLDRED,ARG_ERROR);
        return 1;
  }else{
    char* buffer;
    asprintf(&buffer,"%s%s", "../assets/",(char*)argv[2]);
    menu_options(buffer, (char*)argv[1]);
    free(buffer);
  }
  return 0;
}
