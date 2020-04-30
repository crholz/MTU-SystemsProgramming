/* 
 * The following program has a number of memory leaks. Correct the code so that
 * the program no longer contains a memory leak.
 *
 * Do not change the functionality of the program. You can find out what the
 * program does by running it (it will produce the correct output despite the
 * memory leak).
 *
 * Requirements:
 * 1. The program functionality is not changed.
 * 2. All allocated memory is freed.
 */

/* NAME: Corbin Holz*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reOrder(char **str, char tok){
    int tokPos;
    char *temp;
    int end;
    int i,j;
    char *strPtr;
    
    strPtr=*str;
    end=strlen(strPtr);
    
    tokPos=0;
    while (strPtr[tokPos]!=tok){tokPos++;}
    
    temp=malloc(tokPos+1);
    for (i=0; i<tokPos; temp[i]=strPtr[i],i++);
    temp[tokPos]='\0';
    
    for (i=0; i<(end-tokPos+1); strPtr[i]=strPtr[i+tokPos+1],i++);
    for (i=(end-tokPos-1),j=0; j<tokPos; strPtr[i]=temp[j],j++,i++);
    strPtr[end-1]='\0';
    free(temp);
}

int main() {
    char *my_string;

    my_string = malloc(128);
    strcpy(my_string, "A sample piece of text!");

    printf("Before: %s\n", my_string);
    reOrder(&my_string, 'i');
    printf("After: %s\n", my_string);
    free(my_string);
}
