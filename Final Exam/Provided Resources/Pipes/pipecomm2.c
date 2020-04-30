#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int p2c[2], c2p[2], val = 0, dblval = 0;
main() {
	pipe(p2c); pipe(c2p);
	if (fork() == 0) { /* CHILD */
		close(p2c[1]); close(c2p[0]);
		while(read(p2c[0], &val, sizeof(int)) != 0) {
			dblval = 2*val;
			write(c2p[1], &dblval, sizeof(int));
		}
		exit(0);
	} else { /* PARENT */
		close(c2p[1]); close(p2c[0]);
		for (val = 1; val <= 3; val++) {
			fprintf(stderr, "Asking child to double %d\n", val);
			write(p2c[1], &val, sizeof(int));
			read(c2p[0], &dblval, sizeof(int));
			fprintf(stderr, "Child replied with %d\n", dblval);
		}
		close(p2c[1]); close(c2p[0]);
		wait(NULL);
	}
}
