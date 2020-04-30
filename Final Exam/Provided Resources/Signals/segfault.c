#include<signal.h>
#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>

char buf [120];
char *p = NULL;
int   x;
jmp_buf restore_point;

void handler(int sig)
{
   write(1,"I got a seg fault..\n",strlen("I got a seg fault..\n"));

   p = "Greetings from the handler";

   longjmp(restore_point, sig);
}


int main()
{
   int  cpid,code;

   sigset_t mask,oldmask;

   signal(SIGSEGV,handler);

   printf("PID: %d\n",getpid());
   x=0;
   //sigemptyset(&mask);
   //sigaddset(&mask,SIGINT);
   //sigprocmask(SIG_BLOCK, &mask, &oldmask);

   //sleep(30);

   //write(1,"exiting\n",8);
 
   //sigprocmask(SIG_UNBLOCK, &mask, NULL);
   //x = 1 / x;
   //printf("%d\n",x);
 
   code = setjmp(restore_point);
   dprintf(1,"setjmp executed:%d\n",code);

   x = *p;                                 // Cause a segfault the first time.
   dprintf(1,"%s; code = %d\n",p, code);
}
