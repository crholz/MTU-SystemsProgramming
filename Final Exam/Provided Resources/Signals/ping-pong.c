#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

jmp_buf restore_point1;
jmp_buf restore_point2;

int toggle = 1;

void Handler(int sig)
{
    write(1,"=",1);        // "=" shows that the signal handler was entered.
    if (toggle) 
       {
       toggle = 0;
       siglongjmp(restore_point2, 1);
       }
    else
       {
       toggle = 1;
       siglongjmp(restore_point1, 1);
       }
}


int main()
{
    int  rslt;
    char *p = NULL;
    int  x;
    volatile int k;

    printf("PID is %d\n",getpid());

    // Install our signal handler for USR1 signals
    //
    signal(SIGUSR1,Handler);

    // Set longjmp return point 1. If it returns zero, this is the first call. Note that 
    // when a longjmp is executed it will look like sigsetjmp has returned but the return
    // value will be one (sent from siglongjmp).
    //
    rslt = sigsetjmp(restore_point1,1);

    if (rslt) // We came back here with a longjmp.
       {
       write(1, "Point1-",8);
       while (1) {sleep(1); write(1,"a",1); }
       }

    // Set longjmp return point 2. If it returns zero, this is the first call and we do a 
    // longjmp to return point 1. Otherwise, we continue to return point 2.
    //
    rslt = sigsetjmp(restore_point2,1);

    if (rslt) // We came back here with a longjmp.
       {
       write(1, "Point2-",8);
       while (1) {sleep(1); write(1,"b",1); }   
       }
    else
       siglongjmp(restore_point1,1);

    // This program cannot make it here.
    //
    return 0;
}
