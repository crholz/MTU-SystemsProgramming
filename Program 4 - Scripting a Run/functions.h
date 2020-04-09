#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int write_byte(int newByte, int fd, int translate) {
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
    if (translate == 0) {
        c[byte_count] = newByte;
        byte_count = byte_count + 1;
    }

    else {
        if (newByte < 32 || newByte > 126) {
            unsigned char translated;
            char newLine[5];
            translated = (unsigned char) newByte;

            sprintf(newLine, "<%02x>", translated & 0xFF);

            for (int i = 0; i < 4; i++) {
                write_byte(newLine[i], fd, 0);
            }
        }

        else {
            c[byte_count] = newByte;
            byte_count = byte_count + 1;
        }
    }

    // If bit count is 256, write and restart
    if (byte_count == 256) {
        writeError(write(fd, &c, 256));
        byte_count = 0;
    }
    
    return 0;
}

int write_stdout(int newByte, int fd, int translate) {
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
    if (translate == 0) {
        c[byte_count] = newByte;
        byte_count = byte_count + 1;
    }

    else {
        if (newByte < 32 || newByte > 126) {
            unsigned char translated;
            char newLine[5];
            translated = (unsigned char) newByte;

            sprintf(newLine, "<%02x>", translated & 0xFF);

            for (int i = 0; i < 4; i++) {
                write_stdout(newLine[i], fd, 0);
            }
        }

        else {
            c[byte_count] = newByte;
            byte_count = byte_count + 1;
        }
    }

    // If bit count is 256, write and restart
    if (byte_count == 256) {
        writeError(write(fd, &c, 256));
        byte_count = 0;
    }
    
    return 0;
}

int write_out(int newByte, int fd, int translate) {
    static unsigned char c[256];
    static int byte_count = 0;

    // Upon reset
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
    if (translate == 0) {
        c[byte_count] = newByte;
        byte_count = byte_count + 1;
    }

    else {
        if (newByte < 32 || newByte > 126) {
            unsigned char translated;
            char newLine[5];
            translated = (unsigned char) newByte;

            sprintf(newLine, "<%02x>", translated & 0xFF);

            for (int i = 0; i < 4; i++) {
                write_out(newLine[i], fd, 0);
            }
        }

        else {
            c[byte_count] = newByte;
            byte_count = byte_count + 1;
        }
    }

    // If bit count is 256, write and restart
    if (byte_count == 256) {
        writeError(write(fd, &c, 256));
        byte_count = 0;
    }
    
    return 0;
}

int write_err(int newByte, int fd, int translate) {
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
    if (translate == 0) {
        c[byte_count] = newByte;
        byte_count = byte_count + 1;
    }

    else {
        if (newByte < 32 || newByte > 126) {
            unsigned char translated;
            char newLine[5];
            translated = (unsigned char) newByte;

            sprintf(newLine, "<%02x>", translated & 0xFF);

            for (int i = 0; i < 4; i++) {
                write_err(newLine[i], fd, 0);
            }
        }

        else {
            c[byte_count] = newByte;
            byte_count = byte_count + 1;
        }
    }

    // If bit count is 256, write and restart
    if (byte_count == 256) {
        writeError(write(fd, &c, 256));
        byte_count = 0;
    }
    
    return 0;
}

int write_stderr(int newByte, int fd, int translate) {
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
    if (translate == 0) {
        c[byte_count] = newByte;
        byte_count = byte_count + 1;
    }

    else {
        if (newByte < 32 || newByte > 126) {
            unsigned char translated;
            char newLine[5];
            translated = (unsigned char) newByte;

            sprintf(newLine, "<%02x>", translated & 0xFF);

            for (int i = 0; i < 4; i++) {
                write_stderr(newLine[i], fd, 0);
            }
        }

        else {
            c[byte_count] = newByte;
            byte_count = byte_count + 1;
        }
    }

    // If bit count is 256, write and restart
    if (byte_count == 256) {
        writeError(write(fd, &c, 256));
        byte_count = 0;
    }
    
    return 0;
}