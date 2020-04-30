/* SUPPLEMENTAL FILE -- DO NOT EDIT OR SUBMIT */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    char c;
    int count = 0;
    while (read(6, &c, 1)) count++; 
    printf("%d\n", count);
    return count;
}
