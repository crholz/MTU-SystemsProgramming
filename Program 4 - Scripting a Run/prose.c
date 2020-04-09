#include "functions.h"
#include <string.h>
#include <sys/select.h>

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

    // Create pipes before forking
    int outPipe[2], errPipe[2], outBytesRead, errBytesRead;
    char outBuff[256], errBuff[256];

    pipe(outPipe);
    pipe(errPipe);

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

    // Step 2
    if (fork() == 0) {
        int childNumArgs = argc - 2;
        char *childArgs[childNumArgs + 1];
        char *envpArgs[] = {"newEnvironment", NULL};

        // Close unused sides of the pipe
        close(1);
        close(outPipe[0]);
        close(errPipe[0]);

        // Standard Out Pipe
        dup(outPipe[1]);
        
        close(2);

        //Standard Error Pipe
        dup(errPipe[1]);

        if (outFD < 1)
            error("Error: Error creating file...");


        for (int i = 0; i < childNumArgs; i++)
            childArgs[i] = argv[i + 2];

        childArgs[childNumArgs] = NULL;

        execError(execve(childArgs[0], childArgs, envpArgs));
        close(outPipe[1]);
        close(errPipe[1]);
        return 0;
    }

    // Close Unused Sides of the Pipe
    close(errPipe[1]);
    close(outPipe[1]);


    // Start selection setup
    fd_set readfds;
    struct timeval timeout;
    int maxVal;
    int numRead = 0;


    while(1) {
        FD_ZERO(&readfds);
        FD_SET(outPipe[0], &readfds);
        FD_SET(errPipe[0], &readfds);


        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        maxVal = outPipe[0];
        if (outPipe[0] < errPipe[0])
            maxVal = errPipe[0];


        int selection = select(maxVal + 1, &readfds, NULL, NULL, &timeout);

        switch (selection) {
            case (0):
                // Flush :)
                write_out(256, outFD, 0);
                write_stdout(256, 1, 1);
                write_err(256, errFD, 0);
                write_stderr(256, 2, 1);
                return 0;
                break;

            case (-1):
                
                break;

            default:
                while (FD_ISSET(outPipe[0], &readfds)) {
                    numRead = read(outPipe[0], &outBuff, 256);

                    if (numRead < 256)
                        break;

                    // Write to Standard Out + outfile
                    for (int i = 0; i < numRead; i++) {
                        write_out(outBuff[i], outFD, 0);
                        write_stdout(outBuff[i], 1, 1);
                    }
                }

                // Write to stderr(terminal translated) and file
                while (FD_ISSET(errPipe[0], &readfds)) {
                    numRead = read(errPipe[0], &errBuff, 256);

                    if (numRead < 256);
                        break;

                    // Write to Standard Out + outfile
                    for (int i = 0; i < numRead; i++) {
                        write_err(errBuff[i], errFD, 0);
                        write_stderr(errBuff[i], 2, 1);
                    }
                    
                }

                write_out(256, outFD, 0);
                write_stdout(256, 1, 1);
                write_err(256, errFD, 0);
                write_stderr(256, 2, 1);
                return 0;
                break;
        }

    }

    

}