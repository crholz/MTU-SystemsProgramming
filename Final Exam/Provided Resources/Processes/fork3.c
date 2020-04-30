#include <unistd.h>
#include <stdio.h>

int x = 0;

int main() {
    int y = 0;

	if(fork() == 0) {
		/* Child writes: */
		x++;
        y = 4;
		printf("Child: x=%d y=%d\n", x, y);fflush(stdout);
	}
	else {
		/* Parent writes: */
		x++;
        y = 7;
		printf("Parent: x=%d y=%d\n", x, y);fflush(stdout);
	}
}
