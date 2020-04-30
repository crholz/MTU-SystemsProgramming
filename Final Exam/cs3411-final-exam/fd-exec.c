/*
 * Compile the provided program need6.c into the binary need6.  Complete the
 * following program so that it creates a subprocess that eventually executes
 * the binary need6 in the current directory. 
 *
 * The need6 program simply reads from file descriptor 6, counts the number of
 * bytes that can be read from file descriptor 6, and prints that number on
 * standard output. 
 *
 * Your program must place the file descriptor for datafile (another provided
 * file) in FD 6, then execute need6. Then, it must print the exit value from
 * need6 on its own line. The parent process MAY NOT terminate until need6
 * has completed.
 *
 * The correct output of this program with the included datafile will be
 * 
 * 27
 * 27
 *
 * The first 27 is from need6 printing the value. The second 27 will
 * be from the exit value of need6 being printed by the parent.
 *
 * Error checking is not needed, assume all your calls will succeed.
 *
 * Requirements:
 * 1. The program successfully creates a subprocess and executes need6.
 * 2. need6 successfully prints out the number of bytes in datafile.
 * 3. The program successfully prints out the exit value of need6.
 * 4. The parent must not terminate before need6 exits.
 * 5. You may not add ANY additional includes, the included headers are
 * sufficient to solve the problem.
 * 6. Only submit this file. need6 and datafile will be provided by us
 * during testing.
 */

/* NAME: Corbin Holz*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    /* YOUR SOLUTION GOES HERE */
    int fd = open("datafile", O_RDONLY);
    int waitVal = 0;
    dup2(fd, 6);
    int exitVal = 0;
    if (fork() == 0) {
        char *myArgs[] = {"need6", NULL};
        execve(myArgs[0], myArgs, NULL);
    }
    wait(&waitVal);
    printf("%d\n", WEXITSTATUS(waitVal));

    return 0;
}
