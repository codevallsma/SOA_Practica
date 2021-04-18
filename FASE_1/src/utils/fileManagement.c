#include "fileManagement.h"


/**
 * @brief Function that returns the file descriptor of the file name
 * that has been passed through parameter
 *
 * @param  filename: name of the file
 * @retval fd : file descriptor
 */
int readFileName(char* filename){
    int fd = open(filename, O_RDONLY);
    //Si el fichero no existe o no se ha podido abrir nos vamos
    if (fd < 0) {
        errorSout(filename);
        return -1;
    }

    char basura;

    //Si el fichero está vacío nos vamos
    if (read(fd, &basura, 1) <= 0) {
        printaColors(BOLDRED, ERR_FILE_R);
        close(fd);
        return -1;
    }

    lseek(fd, -1, SEEK_CUR);
    return fd;
}

void read_file_volume_info(int fd){
  if(isEXT2(fd)){
      //read ext2 information
      showEXT2info(fd);
  } else {

  }

}
