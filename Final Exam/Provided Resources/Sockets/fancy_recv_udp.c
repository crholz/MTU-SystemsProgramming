#include <sys/select.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
void printsin(s_in, s1, s2)
struct sockaddr_in *s_in; char *s1, *s2;
{
	printf ("Program: %s\n%s ", s1, s2);
	printf ("(%d,%d)\n", s_in->sin_addr.s_addr, s_in->sin_port);
}

int main() {
	int socket_fd, cc, h_len, fsize, namelen, hits;
	fd_set mask;
	struct timeval timeout;
	struct sockaddr_in s_in, from;

	struct { char head; u_long body; char tail; } msg;

	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero((char *) &s_in, sizeof(s_in)); 
	s_in.sin_family = (short) AF_INET;
	s_in.sin_addr.s_addr = htons(INADDR_ANY);
	s_in.sin_port = htons((u_short)0x3333);
	bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

	for(;;) {
		fsize = sizeof(from);
		FD_ZERO(&mask); FD_SET(0, &mask); FD_SET(socket_fd, &mask);
		timeout.tv_sec = 10; timeout.tv_usec = 0;

		if ((hits = select(socket_fd+1, &mask, (fd_set *)0, 
							(fd_set *)0, &timeout)) < 0) {
			perror("recv_udp:select"); exit(1);
		}

		if( (hits == 0) || ((hits > 0) && (FD_ISSET(0, &mask)))) {
			printf("Shutting down\n"); exit(0);
		}

		cc = recvfrom(socket_fd, &msg, sizeof(msg), 0,
					  (struct sockaddr *)&from, &fsize);
		printsin(&from, "recv_udp: ", "Packet from:");
		printf("Got data ::%c%d%c\n",msg.head,
			ntohl(msg.body),msg.tail);
		fflush(stdout);
	}
}
