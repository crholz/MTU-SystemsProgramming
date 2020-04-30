#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
/* We want to execute "sort < filecomm1.c | grep fprintf | wc" */
main() {	
	int apipe[2];
	int isParent;
	char *cmd[3][3];
	int i, lastChild, fd, saveStdout;
	cmd[0][0] = "sort"; cmd[0][1] = NULL;
	cmd[1][0] = "grep"; cmd[1][1] = "fprintf"; cmd[1][2] = NULL;
	cmd[2][0] = "wc";   cmd[2][1] = NULL;
	saveStdout = dup(1);
	for(i = 2; i >= 0; i--) {
		pipe(apipe);
		isParent = fork();

		if (!isParent) { 
			close(apipe[1]); 
			close(0);
			if(i != 0) { dup(apipe[0]); }
			if(i == 0) {
				fd = open("filecomm1.c", O_RDONLY);
				dup(fd);
			}
			close(apipe[0]);			

			execvp(cmd[i][0],cmd[i]);
			exit(1);
		}
		else {
			if(i==2) lastChild = isParent;			
			close(apipe[0]);
			close(1);
			if(i!=0) { dup(apipe[1]); }			
			close(apipe[1]);					
			if(i==0){ 
				dup2(saveStdout, 1);
				waitpid(lastChild,NULL,0);
			}
		}

	}
}