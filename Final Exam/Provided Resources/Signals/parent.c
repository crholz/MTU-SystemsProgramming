#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
	int cpid;
	int status;

	cpid = fork();
	if (cpid == 0) {
		signal(SIGINT, SIG_IGN);
		printf("Child pid <%d>\n",getpid());
		fflush(stdout);
		execl("sleeper", "sleeper", (char *)0);
		_exit(1);
	} else {
		wait(&status);
	}
}
