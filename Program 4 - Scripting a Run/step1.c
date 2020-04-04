#include <"error.h">
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Step1
// Opens for output the first argument with the added suffix stdout
// Writes the remaining arguments into the file
// Exits
void Step1(char* fileName, int argumentNum, char** arguments) {
    char finalName[100];
    char fileExt[] = ".stdout"

    strcpy(finalName, fileName);
    strcat(finalName, fileExt);

    foError(finalName);

    int fd = open(finalName, open(writeName, O_RDWR | O_CREAT, 0644)
    
    if (argumentNum > 1) {
        char buffer[256];
        for (int i = 1; i < argumentNum; i++) {
            

        }
    }

    close(fd);

}

int main(int argc, char** argv) {

}