#include <stdio.h>

int main(argc, argv, envp)
int argc;
char *argv[], *envp[];
{
	int i; 
	char **ep;
	printf("child is running\n");
	for (i = 0; i < argc; i++) {
		printf("argv[%d]=%s\n",i,argv[i]);
	}
	for (ep = envp; *ep; ep++) {
		printf("%s\n", *ep);
	}
}
