#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int x;
void handler(int sig) {
	x++;
}

int main() {
	int cpid;
	x = 0;

	signal(SIGUSR1,handler);
    
	cpid=fork();

	if (cpid==0) {
		while (x == 0);
        write(1, "Child second.\n",14);
	} else {
		write(1, "Parent first.\n",14);
		/* x++; Not here!!! */
		kill(cpid,SIGUSR1);
		wait(NULL);
		write(1,"Parent third.\n",14);
	}
}
