#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int status;
	if (fork() == 0) {
		sleep(3);
		exit(51);
	} else {
		printf("pid = %d\n", wait(&status));
		printf("status = %x\n", status);
		if (WIFEXITED(status))
			printf("Status (via macro): %d\n", WEXITSTATUS(status));
        sleep(10);
		exit(0);
	}
}
