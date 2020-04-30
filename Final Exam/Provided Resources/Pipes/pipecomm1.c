#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int fd[2], val = 0, dblval = 0;
main() {
	pipe(fd);
	if (fork() == 0) { /* CHILD */
		while(read(fd[0], &val, sizeof(int)) != 0) {
			dblval = 2*val;
			write(fd[1], &dblval, sizeof(int));
		}
		exit(0);
	} else { /* PARENT */
		for (val = 1; val <= 3; val++) {
			fprintf(stderr, "Asking child to double %d\n", val);
			write(fd[1], &val, sizeof(int));
			read(fd[0], &dblval, sizeof(int));
			fprintf(stderr, "Child replied with %d\n", dblval);
		}
		wait(NULL);
	}
}
