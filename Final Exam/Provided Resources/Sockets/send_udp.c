#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>

int main(argc,argv)
int argc; char **argv;
{
	int socket_fd;
	struct sockaddr_in dest;
	struct hostent *hostptr;
	struct { char head; u_long body; char tail; } msgbuf;
	socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
	/* You must do this just in case */
	bzero((char *) &dest, sizeof(dest));

	hostptr = gethostbyname(argv[1]);
	dest.sin_family = (short) AF_INET;
	bcopy(hostptr->h_addr, (char *)&dest.sin_addr,
		hostptr->h_length);
	dest.sin_port = htons((u_short)0x3333);
	msgbuf.head = '<';
	msgbuf.body = htonl(getpid()); /* IMPORTANT! */
	msgbuf.tail = '>';
	sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,
		(struct sockaddr *)&dest, sizeof(dest));

    printf("My PID was: %d\n", getpid());
}
