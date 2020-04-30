#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(argc, argv)
int argc;char *argv[];
{
	int myfd;
	char gotch, val;
	if (argc != 3) {
		fprintf(stderr,"Usage: pchild fd charnum\n");
		exit(1);
	}
	myfd = atoi(argv[1]);
	gotch = atoi(argv[2]);

	lseek(myfd, (off_t)gotch, SEEK_SET);

    if(read(myfd, &val, 1) != -1)
    	printf("Child got char %d from fd %d: %c\n", gotch, myfd, val);
    else
        printf("Error\n");
}
