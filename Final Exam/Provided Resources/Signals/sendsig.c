#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void usr1handler() {
	write(1, "\nOuttahere\n", 11);
	exit(0);
}

int main() {
	int kidpid;

	signal(SIGUSR1, SIG_IGN);

	if ((kidpid=fork()) == 0) {
		execl("catchsig", "catchsig", (char *)0);
	}
	else {
		sleep(3);
		kill(kidpid,SIGUSR1);
		wait(NULL);
		fprintf(stderr, "Sendsig detects death of catchsig\n");
		exit(0);
	}
	exit(1);
}
