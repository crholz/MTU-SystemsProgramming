#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
void printsin(s_in, s1, s2)
struct sockaddr_in *s_in; char *s1, *s2;
{
	printf ("Program: %s\n%s ", s1, s2);
	printf ("(%d,%d)\n", s_in->sin_addr.s_addr, s_in->sin_port);
}
int main() {
	int listener, conn, length; char ch;
	struct sockaddr_in s1, s2;
	
	listener = socket( AF_INET, SOCK_STREAM, 0 );
	
	bzero((char *) &s1, sizeof(s1));
	s1.sin_family = (short) AF_INET;
	s1.sin_addr.s_addr = htonl(INADDR_ANY);
	s1.sin_port = htons(0); /* bind() will gimme unique port. */
	bind(listener, (struct sockaddr *)&s1, sizeof(s1));
	length = sizeof(s1);
	getsockname(listener, (struct sockaddr *)&s1, (socklen_t*) &length);
	printf("RSTREAM:: assigned port number %d\n", 
			ntohs(s1.sin_port));

	listen(listener,1);
	length = sizeof(s2);
	conn=accept(listener, (struct sockaddr *)&s2, (socklen_t*) &length);
	printsin(&s2,"RSTREAM::", "accepted connection from");
	printf("\n\nRSTREAM:: data from stream:\n");
	while ( read(conn, &ch, 1) == 1) printf("%c", ch);
	printf("\n");
}
