#include "error.h"
#include <sys/select.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void setToBlank(char* stringPointer) {
    stringPointer = "";
    return;
}

int main(int argc, char** argv) {
    fd_set readfds;
    struct timeval timeout;
    int fd = 0;


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
                do {
                    write(1, &readFrom, 256);
                    setToBlank(readFrom);
                    readChar = read(0, &readFrom, 256);
                } while (readChar != 0);

                return 0;
        }
    }
    
    
}