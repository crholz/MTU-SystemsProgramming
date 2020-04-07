#include "error.h"
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;

    char pipeName[] = "transit";

    while (1) {
        fd = open(pipeName, O_WRONLY, 0644);

        unsigned char buff[256];
        int readFrom = 0;

        readFrom = read(1, &buff, 256);
        write(fd, &buff, readFrom);
        
        close(fd);
    }

    return 0;
}