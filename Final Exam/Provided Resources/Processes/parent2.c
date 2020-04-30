#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(argc, argv)
int argc; char *argv[];
{
	int forkid, charnum;
	char fdval[20];
	if (argc != 3) {
		fprintf(stderr,"Usage: pexec filename charnum\n");
		exit(1);
	}
	if ((forkid = open(argv[1], O_RDONLY)) < 0 ) {
		fprintf(stderr, "Cannot open %s\n", argv[1]);
		exit(2);
	}
	sprintf(fdval, "%d", forkid); /* sprintf! */
	if (fork() == 0) {
		execl("pchild", "pchild", fdval, argv[2], (char *)0);
		fprintf(stderr, "Unable to exec\n");
		exit(3);
	}
	printf("Parent is after fork/exec\n");
}
