#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fdin, fdout;
    char *src, *dst;
    struct stat statbuf;

    if (argc < 3) {
        printf("Usage: mcopy <source> <dest>\n");
        return -1;
    }

    fdin = open(argv[1], O_RDONLY);
    fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    fstat(fdin, &statbuf);
    // Set output file size
    lseek(fdout,statbuf.st_size - 1, SEEK_SET);
    write(fdout, "", 1);

    src = mmap(0, statbuf.st_size, PROT_READ, 
            MAP_SHARED, fdin, 0);
    dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, 
            MAP_SHARED, fdout, 0);

    memcpy(dst, src, statbuf.st_size);

    return 0;
}
