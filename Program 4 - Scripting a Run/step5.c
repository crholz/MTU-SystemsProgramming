#include "error.h"
#include <sys/select.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void setToBlank(char* stringPointer) {
    stringPointer = "";
    return;
}

int main(int argc, char** argv) {
    fd_set readfds;
    struct timeval timeout;
    int fd = 0;

    // Step 5
    if (fork() == 0) {
        fd_set childSet;
        struct timeval childTime;
        char pipeName[] = "transit";
        int transitPipe = open(pipeName, O_RDONLY, 0644);

        if (transitPipe < 0)
            if (mkfifo(pipeName, 0666) == -1)
                error("Error: Could not create pipe.");

        int parentPID = getppid();

        close(transitPipe);
	int myKill = kill(parentPID, 0);
        while(myKill == 0) {
	    myKill = kill(parentPID, 0);
            transitPipe = open(pipeName, O_RDONLY, 0644);

            FD_ZERO(&childSet);
            FD_SET(transitPipe, &childSet);

            childTime.tv_sec = 0;
            timeout.tv_usec = 10000;

            int childSelect = select(transitPipe + 1, &childSet, NULL, NULL, &childTime);
            unsigned char pipeBuf[256];
            int fromPipe = 0;

            switch(childSelect) {
                case (-1):
                    error("Error establishing case...");
                    break;
                
                case (0):
                    break;

                default:
                    fromPipe = read(transitPipe, &pipeBuf, 256);
                    write(1, &pipeBuf, fromPipe);
                    setToBlank(pipeBuf);
                    break;

            }
            close(transitPipe);
        }
        write(1, "dead", 4);
        return 0;
    }


    while(1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int selection = select(fd + 1, &readfds, NULL, NULL, &timeout);
        char tick[] = "tick";
        int readChar = 0;
        unsigned char readFrom[256] = "";

        switch (selection) {
            case (-1):
                error("Error: Error during selection");
                break;
    
            case (0):
                write(1, &tick, 4);
                break;
    
            default:
                readChar = read(0, &readFrom, 256);
                write(1, &readFrom, readChar);
                setToBlank(readFrom);

        }
    }
    
    
}
