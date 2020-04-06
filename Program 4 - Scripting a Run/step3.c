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

    int fd = open(finalName, O_RDWR | O_CREAT | O_APPEND, 0644);
    
    for (int i = 2; i < argumentNum; i++) {
        for (int j = 0; j < strlen(arguments[i]); j++) {
            write_byte(arguments[i][j], fd);
        }
        if (i != argumentNum - 1) {
            write_byte(' ', fd);
        }
    }

    write_byte(256, fd);

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
        for (int i = 0; i < childNumArgs; i++)
            childArgs[i] = argv[i + 2];

        childArgs[childNumArgs] = NULL;

        execError(execve(childArgs[0], childArgs, envpArgs));
        return;
    }
}