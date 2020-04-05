#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int write_byte(int newByte, int fd) {
    static unsigned char c[256];
    static int byte_count = 0;

    if (byte_count == 0) {
        for (int i = 0; i < 256; i++) {
            c[i] = 0;
        }
    }
    
    // Flush for bitcount
    if (newByte == 256) {
        writeError(write(fd, &c, byte_count));
        return 0;
    }

    // If not flushed
    c[byte_count] = newByte;
    byte_count = byte_count + 1;

    // If bit count is 256, write and restart
    if (byte_count == 256) {
        writeError(write(fd, &c, 256));
        byte_count == 0;
    }
    
    return 0;
}