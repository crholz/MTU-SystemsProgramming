#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
 * findInDict
 * Look through the dictionary
 * @find the character to find in the dicitonary
 * @diction the dictionary to look through
 * @returns the index where the character is found or -1 if it is not
 */
int findInDict(unsigned char find, unsigned char * diction) {
    int found = -1;

    for (int i = 1; i < 16; i++) {
        if (find == diction[i]) {
            found = i;
            return found;
        }
    }

    return found;
}

// to return one byte from the standard input
int read_byte() {
    static int buf_count = 0;
    static int buf_index = 0;
    static char buf[256];
    char bufbuf[300];
    unsigned char c;
    if (buf_count == 0) { // nothing in buffer
        buf_count = read(0, buf, sizeof(buf));
        buf_index = 0;
    }

    if (buf_count == 0) {
        return 256;
    }
    
    if (buf_count < 0) {
        perror("read failed:");
        exit(-1);
    }

    c = buf[buf_index];
    buf_index++;
    buf_count--;

    return c;
}

int write_byte(int input, int fd) {
    static int buf_count = 0;
    static int buf_index = 0;
    static char buf[256];
    char c;

    if (input == 256) {
        write(fd, buf, buf_count);
        buf_count = 0;
        buf_index = 0;
    } else {
        if (buf_count < 256) {
            buf[buf_index] = (char) input;
            buf_index++;
            buf_count++;
        }

        if (buf_count == 256) {
            write(fd, buf, 256);
            buf_count = 0;
            buf_index = 0;
        }
    }

    return 0;
}

int read_bit() {
    static unsigned char c;
    int temp_byte;
    static int bit_count = 0;
    int bit;

    if (bit_count == 0) {
        temp_byte = read_byte();
        bit_count = 8;
        c = (unsigned char) temp_byte;
        if (temp_byte == 256) {
            return 256;
        }
    }

    bit = (c >> (bit_count-1)) & 1;
    bit_count--;
    return bit;
}

int write_bit(int bit, int fd) {
    static unsigned char c = 0;
    static int bit_count = 0;

    if (bit == 256) {
        // missing padding code here: if bit_count < 8 you must add enough 0s

        if (bit_count == 0) {
            return 0;
        }

        while (bit_count < 8) {
            c = (c << 1) | 0;
            bit_count++;
        }

        write_byte(c, fd);
        bit_count = 0;
        return 0;
    }

    c = (c << 1) | (bit & 1);
    bit_count++;

    if (bit_count == 8) {
        write_byte(c, fd);
        bit_count = 0;
    }
    
    return 0;
}