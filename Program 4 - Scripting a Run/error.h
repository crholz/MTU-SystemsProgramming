#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * error
 * Write error to standard out.
 */
int error(char* myError)
{
	char buffer[512];
	sprintf(buffer, "%s", myError);
	write(1, buffer, strlen(buffer));
	exit(1);
}

/*
 * argError
 * Check to make sure the number of arguments are correct 
 * @numArgs the number of arguments passed in
 * @neededArgs the amount of arguments needed for function
 */
void argError(int numArgs, int neededArgs) {
    if (numArgs < neededArgs)
        error("Error: Not Enough Arguments\n");

}

/*
 * readError
 * Check to make sure can read the file
 * @bytesRead
 */
void readError(int bytesRead) {
    if (bytesRead < 0)
        error("Error: Could not read file\n");

}

/*
 * foError
 * Try to open the file
 * @filename name of file to open
 * @read if read == 1 then try to read
 */
void foError(char* filename, int tryRead) {
    int fd;
    char * buf;

    fd = open(filename, O_RDONLY, 0644);

    if (fd > 0)
        error("Error: File Already Exists.\n");

    close(fd);
}

void wfError(char* filename) {
    int fd = open(filename, O_RDONLY, 0644);

    if (fd > 0)
        error("Error: File already exists.\n");

    close(fd);
}