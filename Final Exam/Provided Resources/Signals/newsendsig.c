#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	int kidpid, status;

	if ((kidpid=fork()) == 0) {
		execl("newcatchsig", "newcatchsig", (char *)0);
	}
	else {
		sleep(5);
		kill(kidpid,SIGUSR1);
		sleep(5);
		kill(kidpid,SIGINT);
		wait(&status);
		fprintf(stderr, "Sendsig detects death of catchsig\n");
		if (WIFSIGNALED(status))
			fprintf(stderr, "Died due to uncaught sig %d\n"
				,WTERMSIG(status));
		exit(0);
	}
	exit(1);
}
