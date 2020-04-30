/*
 * Modify the following program so that it prints the same output
 * but terminates correctly instead of hanging. You may NOT remove
 * the call to wait(NULL) to accomplish this -- it must be the last
 * line of code in the else {} block.
 *
 * Hint: This is achievable by adding one simple line of code.
 *
 * Error checking is not needed, assume all your calls will succeed.
 *
 */

/* NAME: Corbin Holz*/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int pfd[2],ret,i,fd;
    char ch;

    pipe(pfd);
    if (fork()==0){   
        close(pfd[1]);
        while (read(pfd[0],&ch,1)!=0) write(1,&ch,1);
    } else {
        close(pfd[0]);
        fd=open("datafile",O_RDWR);
        for (i=0;i<5;i++) {
            read(fd,&ch,1);
            write(pfd[1],&ch,1);
        }
        //write(pfd[1],0,1);
        close(pfd[1]);
        wait(NULL);
    }
}
