#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

int main(void) {
	struct termios ts, ots;
	char passbuf[1024];

	/* get and save current termios settings */
	tcgetattr(0, &ts);
	ots = ts;

	/* change and set new termios settings */
	ts.c_lflag &= ~ECHO;
	ts.c_lflag |= ECHONL;
	tcsetattr(0, TCSAFLUSH, &ts);

	/* paranoia: check that the settings took 
	effect */
	tcgetattr(0, &ts);
	if (ts.c_lflag & ECHO) {
		fprintf(stderr, "Failed to turn off echo\n");
		tcsetattr(0, TCSANOW, &ots);
		exit(1);
	}

	if (mlock(passbuf, 1024) == -1)
		perror("mlock");

	printf("Enter password: ");
	fflush(stdout);
	fgets(passbuf, 1024, stdin);
	printf("Read password: %s", passbuf);

	bzero(passbuf, 1024);
	if (munlock(passbuf, 1024) == -1)
		perror("munlock");

	tcsetattr(0, TCSANOW, &ots);

	exit(0);
}
