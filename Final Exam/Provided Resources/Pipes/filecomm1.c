#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
main() {
	int fd, val, dblval;
	fd = open("commofile", O_RDWR|O_CREAT|O_TRUNC, 0644);
	if (fork() == 0) { /* CHILD */
		read(fd, &val, sizeof(int));
		lseek(fd, 0, SEEK_SET);
		dblval = 2 * val;
		write(fd, &dblval, sizeof(int));
		lseek(fd, 0, SEEK_SET);
		exit(0);
	} else { /* PARENT */
		val = 2;
		fprintf(stderr, "Asking child to double %d\n", val);
		write(fd, &val, sizeof(int));
		lseek(fd, 0, SEEK_SET);
		wait(NULL);
		read(fd, &dblval, sizeof(int));
		fprintf(stderr, "Child replied with %d\n", dblval);
		exit(0);
	}
}
