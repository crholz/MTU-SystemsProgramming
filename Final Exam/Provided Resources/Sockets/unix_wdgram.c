#include <errno.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
	int socket_fd, cc;
	//long getpid();
	struct sockaddr_un dest;
    struct sockaddr_un mine;

	struct {
		char head;
		u_long body;
		char tail;		
	} msgbuf;

	socket_fd = socket (AF_UNIX, SOCK_DGRAM, 0);
	dest.sun_family = AF_UNIX;
	strcpy(dest.sun_path, "udgram");

    mine.sun_family = AF_UNIX;
    strcpy(mine.sun_path, "mynewaddr");

    bind(socket_fd, (struct sockaddr *)&mine, sizeof(mine));

	msgbuf.head = '<';
	msgbuf.body = (u_long) getpid();
	msgbuf.tail = '>';

	cc = sendto(socket_fd, &msgbuf, sizeof(msgbuf),0, 
		(struct sockaddr *)&dest,sizeof(dest));
}
