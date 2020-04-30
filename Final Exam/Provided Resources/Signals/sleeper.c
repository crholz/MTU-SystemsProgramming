#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void handler() {
	printf("Sleeper exits on SIGTERM\n");
	fflush(stdout);
}

int main() {
	signal(SIGTERM, handler);
	pause();
}
