#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SIZE sizeof(long long int)
#define NLOOPS 20

int main() {
    int fd, i, counter;
    pid_t pid;
    long long int *area;

    fd = open("/dev/zero", O_RDWR);
    area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    *area = 0;

    pid = fork();

    if (pid > 0) {
        // Parent process
        for (i = 0; i < NLOOPS; i += 2) {
            while (*area != i); // empty loop
            // won't print unless *area is even
            printf("<P> sees value <%lld>\n", *area);
            fflush(stdout);
            *area += 1;
        }
    } else {
        // Child process
        for (i = 1; i < NLOOPS + 1; i += 2) {
            while (*area != i); // empty loop
            // won't print unless *area is odd
            printf("<C> sees value <%lld>\n", *area);
            fflush(stdout);
            *area += 1;
        }
    }
    return 0;
}
