/*
 * The program below takes in 2 one character arguments, and prints them one
 * after the other.  For instance, if the program is run like so:
 * 
 * ./signals a b
 *
 * It will print:
 *
 * abababab...
 *
 * Printing one character per second. Modify the program below so that it
 * prints only the first character every second until it receives SIGUSR1.
 * After receiving SIGUSR1, it should only print the second character every
 * second. If it receives another SIGUSR1, it should go back to printing
 * the first character, and so on.
 *
 * Example expected output:
 * aaaaaaabbbbbbaaaabbbb...
 *       ^     ^   ^
 *
 * The ^ symbols indicate where the SIGUSR1 signal was received by the process.
 *
 * Error checking is not needed, assume all your calls will succeed.
 * 
 * Requirements:
 * 1. The program successfully produces the expected output.
 * 2. You may add functions or variables to this program as necessary.
 * 3. You may not add ANY additional includes, the included headers are
 * sufficient to solve the problem.
 *
 */

/* NAME: Corbin Holz*/

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int toggle = 1;

void Handler(int sig)
{
    if (toggle) 
       {
       toggle = 0;
       }
    else
       {
       toggle = 1;
       }
}

int main(int argc, char **argv) {
    char first, second;

    if (argc < 3) { 
        printf("Not enough arguments!\n");
        printf("Usage: ./signals a b\n");
        printf("where a and b are both single characters\n");
        exit(-1);
    }

    first = argv[1][0]; // hint: you don't have to modify these!
    second = argv[2][0];

    signal(SIGUSR1,Handler);

    while (1) {
        if (toggle) {
            write(1, &first, 1);
            sleep(1);
        }
        
        else {
            write(1, &second, 1);
            sleep(1);
        }
    }

    return 0;
}
