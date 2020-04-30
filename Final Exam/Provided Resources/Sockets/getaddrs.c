#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(argc,argv)
int argc; char **argv;
{
	struct hostent *entry; char **next; 
	struct in_addr address, **addrptr;

	entry = gethostbyname(argv[1]);

	if (!entry) { herror("lookup error"); exit(1); }

	printf("Official name -> %s\n", entry->h_name);

	if (entry->h_aliases[0]) {
		printf("Aliases ->\n");
		for (next = entry->h_aliases; *next; next++)
			printf(" %s\n", *next);
	}

	printf("IP Addresses:\n");

	for (addrptr=(struct in_addr **) entry->h_addr_list; 
		*addrptr; addrptr++) {
		printf(" %s\n", inet_ntoa(**addrptr));
    }
}
