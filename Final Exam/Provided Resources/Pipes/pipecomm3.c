#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
/* We want to execute "ps aux | grep galolu" */
main() {
	int isParent;
	int apipe[2];
	char *cmd[2][3];

	cmd[0][0] = "ps";   cmd[0][1] = "aux"; cmd[0][2] = NULL;
	cmd[1][0] = "grep"; cmd[1][1] = "galolu"; cmd[1][2] = NULL;

	pipe(apipe);
	isParent = fork();

	if (!isParent) { /* Child is going to be "grep galolu" */					
		/* We want stdin connected to our pipe! */
		close(apipe[1]); 
		close(0);
		dup(apipe[0]);
		close(apipe[0]);

		execvp(cmd[1][0],cmd[1]);
		perror("Child exec::"); exit(1);
	}
	else { /* Parent is "ps aux" */
	/* We want the stdout connected to pipe */
		close(apipe[0]);
		close(1);
		dup(apipe[1]);
		close(apipe[1]);

		execvp(cmd[0][0], cmd[0]);
		perror("Parent exec::"); exit(1);
	}
}
