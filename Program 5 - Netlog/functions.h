#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int writeString(char stringIn[], int writeOut) {
    int length = strlen(stringIn);

    char buff [256];
    int charsRead;
    int counter = 0;
    int i = 0;
    

    if (length == 0)
        return 0;

    while (counter < length) {
        for (i = 0; i < 256; i++) {
            // end of the string, write
            if (i == length - 1) {
                write(writeOut, &buff, i);
                return 0;
            }
            buff[i] = stringIn[counter];
            counter++;
        }
        write(writeOut, &buff, 256);
    }

    return 0;
}

