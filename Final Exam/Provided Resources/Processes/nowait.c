#include <unistd.h>
#include <stdlib.h>
int main() {
	if (fork() == 0) {
		exit(1);
	} else {
		sleep(10);
		exit(1);
	}
}
