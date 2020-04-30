#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int flag = 1;

void usr1handler() {
	flag = 0;
}

int main() {
	signal(SIGUSR1, usr1handler);
	while(flag) { sleep(1); write(1,"A",1);}
	write(1, "\nEscaped Loop\n", 14);
	flag = 1;
	while(flag) { sleep(1); write(1,"B",1);}	
	write(1, "\nEscaped Loop\n", 14);
	exit(0);
}
