/* 
 * The following code attempts to copy a string and then print the copy.
 * Execution of the program results in a segmentation fault. (A segmentation
 * fault indicates that a program has tried to access memory outside its
 * address space.) Identify the source of the segmentation fault and fix it.
 * Name: Corbin Holz
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copyString(char *src, char *dest, int size)
{
    int index;
    index=0;
    while ((*src != '\0') && (index < size)){
        *dest= *src;
        src++;dest++;index++;
    }
}

int main(){
    char sourceStr[]="ABCDEFG";

    // Memory needs to be allocated to run
    char *buf = malloc(sizeof(sourceStr));
    copyString(sourceStr, buf,strlen(sourceStr));
    printf("%s\n",buf);
    free(buf);
}
