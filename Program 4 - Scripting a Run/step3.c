#include "functions.h"
#include <string.h>

// Step1
// Opens for output the first argument with the added suffix stdout
// Writes the remaining arguments into the file
// Exits
void Step1(char* fileName, int argumentNum, char** arguments) {
    char finalName[256];
    char fileExt[] = ".stdout";

    strcpy(finalName, fileName);
    strcat(finalName, fileExt);

    foError(finalName);

    char errorName[256];
    char errorExt[] = ".stderr";

    strcpy(errorName, fileName);
    strcat(errorName, fileExt);

    foError(errorName);

    int fd = open(finalName, O_RDWR | O_CREAT | O_APPEND, 0644);
    
    for (int i = 2; i < argumentNum; i++) {
        for (int j = 0; j < strlen(arguments[i]); j++) {
            write_byte(arguments[i][j], fd, 0);
        }
        if (i != argumentNum - 1) {
            write_byte(' ', fd, 0);
        }
    }

    write_byte(256, fd, 0);

    close(fd);

}

int main(int argc, char** argv) {
    argError(argc, 2);
    Step1(argv[1], argc, argv);

    // Step 2
    if (fork() == 0) {
        int childNumArgs = argc - 2;
        char *childArgs[childNumArgs + 1];
        char *envpArgs[] = {"newEnvironment", NULL};

        // Step 3
        char fileExt[] = ".stdout";
        char finalName[256];
        strcpy(finalName, argv[1]);
        strcat(finalName, fileExt);
        int outFD = open(finalName, O_RDWR | O_APPEND, 0644);

        char errorName[256];
        char errorExt[] = ".stderr";

        strcpy(errorName, argv[1]);
        strcat(errorName, errorExt);
        int errFD = open(errorName, O_RDWR | O_APPEND | O_CREAT, 0644);

        close(1);
        dup(outFD);
        close(2);
        dup(errFD);

        if (outFD < 1)
            error("Error: Error creating file...");


        for (int i = 0; i < childNumArgs; i++)
            childArgs[i] = argv[i + 2];

        childArgs[childNumArgs] = NULL;

        execError(execve(childArgs[0], childArgs, envpArgs));
        return 0;
    }
}