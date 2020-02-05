#include <stdio.h>

int main(int argc, char** argv) {
	char *a;
	float f = 79.7;

	a = (char *) &f;

	printf("float: %f\n", f);

	printf("Byte Array: %hhX:%hhX:%hhX:%hhX\n", a[0], a[1], a[2], a[3]);

	char temp = a[3];
	a[3] = a[0];
	a[0] = temp;

	temp = a[1];
	a[1] = a[2];
	a[2] = temp;

	printf("float: %f\n", f);

	printf("Byte Array: %hhX:%hhX:%hhX:%hhX\n", a[0], a[1], a[2], a[3]);

	return 0;
}
