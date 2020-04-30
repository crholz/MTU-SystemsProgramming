#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(argc, argv)
int argc;char *argv[];
{
	int myfd;
	char gotch, val;
	if (argc != 2) {
		fprintf(stderr,"Usage: piokid charnum\n");
		exit(1);
	}
	gotch = atoi(argv[1]);
	lseek(0, (off_t)gotch, SEEK_SET);
	read(0, &val, 1);
	printf("Child got char %d from stdin: %c\n", gotch, val);
}
