#include <unistd.h>

int main() {
	if(fork() == 0) {
		/* Child writes: */
		write(1, "Hi \n", 4);
	}
	else {
		/* Parent writes: */
		write(1, "Hey \n", 5);
	}
}
