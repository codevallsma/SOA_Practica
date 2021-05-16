#include "Utils.h"

/**
 * @brief  Reads until character passed by parameter from the respective file
 * and returns the number read in an int type
 * 
 * @param  file: file from where to read
 * @param  charReadUntil: character end
 * @retval int read
 */
int readUntilAtoiAndFree(int file, char charReadUntil) {
    char *string = readUntil(file, charReadUntil);
    int valor = atoi(string);
    free(string);
    return valor;
}

/**
 * @brief  Reads until character passed by parameter from the respective file
 * and returns char* with the content read
 * 
 * @param  fd: file descriptor
 * @param  end: character end
 * @retval content that has been read
 */
char *readUntil(int fd, char end) {
    int i = 0;
    char c = '\0';
    char *string = (char *) malloc(sizeof(char));
    int status = 1;

    while (c != end && status != 0) {
        status = read(fd, &c, sizeof(char));
        if (c != end) {
            string = (char *) realloc(string, sizeof(char) * (i + 2));
            string[i] = c;
        }
        i++;
    }

    string[i - 1] = '\0';
    return string;
}

/**
 * Recieves an static array and fills it until it finds a character or reaches the length of the char array
 * @param fd : the file descriptor
 * @param staticArray : The array to be filled
 * @param end : The character that limits until where we have to read
 */
void readUntilStatic(int fd, u_char staticArray[], int totalSize, char end){
    int i = 0;
    char c = '\0';
    int status = 1;

    while (c != end && status != 0 && i<totalSize) {
        status = read(fd, &c, sizeof(char));
        if (c != end) {
            staticArray[i] = c;
        } else{
            //we found the char delimitter
            status =0;
        }
        i++;
    }
    staticArray[i-1] = '\0';
}

/**
 * @brief  Reads until a concrete size from the file descriptor
 *
 * @param  fd: file descriptor
 * @param  size: size to read
 * @retval content read
 */
char *readUntilNoMore(int fd, int *size) {
    int i = 0;
    char c = '\0';
    char *string = (char *) malloc(sizeof(char));

    while (read(fd, &c, sizeof(char)) != 0) {
        string = (char *) realloc(string, sizeof(char) * (i + 2));
        string[i] = c;
        i++;
    }
    *size = i;
    string[i - 1] = '\0';
    return string;
}

/**
 * @brief  Reads float from file
 *
 * @param  file: file descriptor
 * @param  charReadUntil: character until where it has to read
 * @retval float read from file
 */
float readFloat(int file, char charReadUntil) {
    char *string = readUntil(file, charReadUntil);
    float valor = atof(string);
    free(string);
    return valor;
}

/**
 * @brief  Passes to lower case char* passed by parameter
 * 
 * @param  s: char*
 * 
 */
void toLower(char *s) {
    for (char *p = s; *p; p++) *p = tolower(*p);
}

/**
 * @brief  Passes to lower case char* passed by parameter
 *
 * @param  s: char*
 *
 */
void toUpper(char *s) {
    for (char *p = s; *p; p++) *p = toupper(*p);
}
/**
 * @brief  Function that does an itoa depending on the base
 * 
 * @param  val: int to be passed to char*
 * @param  base: base
 * @retval char* of int
 */
char *itoaCustom(int val, int base) {

    static char buf[32] = {0};
    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];

}

/**
 * @brief  Copies content from a char[] to another
 * 
 * @param  destinationArr: char[] where it has to be copied the other char[]
 * @param  sourceArr[]: char[] that will be copied
 * @param  shortestSize: source array size
 * @param  longestSize: destination size
 * 
 */
void copyContentArray(char destinationArr[], char sourceArr[], int shortestSize, int longestSize) {
    for (int i = 0; i < longestSize; i++) {
        if (i < shortestSize) {
            destinationArr[i] = sourceArr[i];
        } else {
            destinationArr[i] = '\0';
        }
    }
}

