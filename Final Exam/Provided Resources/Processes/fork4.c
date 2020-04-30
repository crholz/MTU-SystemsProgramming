#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
	int fd; char ch1, ch2, ch3;
	fd = open("datafile",O_RDWR);

	read(fd,&ch1,1);

	printf("Before fork: ch1 = %c\n", ch1);fflush(stdout);

	if (fork() == 0) {
		/* Child */
		read(fd,&ch2,1);
		printf("In child: ch2 = %c\n", ch2);fflush(stdout);
        close(fd);
	}
    else {
        /* Parent */
		read(fd,&ch3,1);
		printf("In parent: ch3 = %c\n", ch3);fflush(stdout);
        close(fd);
    }
}
