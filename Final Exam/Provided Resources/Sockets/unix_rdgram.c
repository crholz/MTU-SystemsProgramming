#include <errno.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
	short p_len;
	int socket_fd, cc, h_len, fsize, namelen;
	void printsun();
	struct sockaddr_un s_un, from;
	size_t addrlength;

	struct {
		char head;
		u_long body;
		char tail;
	} msg;

	socket_fd = socket (AF_UNIX, SOCK_DGRAM, 0);

	s_un.sun_family = AF_UNIX;
	strcpy(s_un.sun_path, "udgram");
	addrlength = sizeof(s_un.sun_family) + sizeof(s_un.sun_path);
	unlink("udgram");

	bind(socket_fd, (struct sockaddr *)&s_un, addrlength);

	for(;;) {
		fsize = sizeof(from);
		cc = recvfrom(socket_fd, &msg, sizeof(msg), 0, 
			(struct sockaddr *)&from, &fsize);
		printsun(&from, "unix_rdgram: ", "Packet from");
		printf("Got data ::%c%ld%c\n", msg.head,msg.body,msg.tail);
		fflush(stdout);
	}
}

void printsun(Sun, s1, s2)
struct sockaddr_un *Sun; char *s1, *s2;
{
	printf("%s %s\n",s1,s2);
	printf("      family <%d> addr <%s>\n", 
		Sun->sun_family, Sun->sun_path);
}
