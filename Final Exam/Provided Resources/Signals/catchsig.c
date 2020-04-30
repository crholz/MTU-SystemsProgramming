#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void usr1handler() {
	write(1, "\nOuttahere\n", 11);
	exit(0);
}

int main() {
	/* Embed the handler */
	signal(SIGUSR1, usr1handler);
	signal(SIGINT, usr1handler);
	while(1) {
		sleep(1);
		write(1,"A",1);
	}
}
