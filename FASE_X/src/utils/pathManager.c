#include "pathManager.h"

/**
 * Copying the directory name to the path array so we can later display the path in the reverse order
 * @param num_of_subdirs
 * @param dirName
 * @param paths
 */
char**  copyDirPaths(char* dirName, char** paths, int* num_of_subdirs){
    *num_of_subdirs = *num_of_subdirs +1;
    //if it is the first time we want to allocate memmory we do a malloc
    if(*num_of_subdirs ==1 ){
        paths = (char**)malloc((*num_of_subdirs+1)*sizeof (char*));
    } else {
        //if it is the second time we want to allocate memmory we do a realloc
        paths = (char**)realloc(paths,(*num_of_subdirs+1)*sizeof (char*));
    }
    //we do a malloc for the char contents
    paths[*num_of_subdirs-1] = (char*)malloc((strlen(dirName)+1)*sizeof (char*));
    //copying the content to the destination path array
    strcpy(paths[*num_of_subdirs-1], dirName);
    return  paths;
}
/**
 * Freeing the contents of the char ** path variable
 * @param paths
 */
void freePath(char** paths, uint subdirs){
    for(uint i =0; i < subdirs; i++){
        free(paths[i]);
    }
    free(paths);
}