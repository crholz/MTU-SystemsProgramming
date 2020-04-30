#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	char *a[5], *e[3];

	a[0] = "child";
	a[1] = "argument1";
	a[2] = "argument2";
	a[3] = "argument3";
	a[4] = NULL;

	e[0] = "ENV0=val0";
	e[1] = "ENV1=val1";
	e[2] = NULL;

	execve("child", a, e);
	/* If we get here, something went wrong */
	perror("parent1");
	exit(1);
}
