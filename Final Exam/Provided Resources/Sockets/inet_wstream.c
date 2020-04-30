#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
char msg[] = { "the quick brown fox jumps over the lazy dog" };
int main(int argc, char **argv) {
	char *remhost; unsigned short remport;
	int sock, left, num, put;
	struct sockaddr_in remote;
	struct hostent *h;

	remhost = argv[1]; remport = atoi(argv[2]);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *) &remote, sizeof(remote));
	remote.sin_family = (short) AF_INET;
	h = gethostbyname(remhost);
	bcopy((char *)h->h_addr, (char *)&remote.sin_addr, 
		  h->h_length);
	remote.sin_port = htons(remport);
	connect(sock, (struct sockaddr *)&remote, sizeof(remote));

	left = sizeof(msg); put = 0;
	while(left > 0) {
		if((num = write(sock, msg+put, left)) < 0) {
			perror("inet_wstream: write");
			exit(1);
		}
		else {
			left -= num;
			put += num;
		}
	}
}
