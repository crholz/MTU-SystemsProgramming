#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "error.h"

// Create a server which can obtain a TCP port and print the port number on STD OUT
struct sockaddr_in *s_in; char* s1, * s2;


int main(int argc, char** argv) {
    int listener, conn, length; char ch;

    struct sockaddr_in s1, s2;

    listener = socket( AF_INET, SOCK_STREAM, 0);

    bzero((char *) &s1, sizeof(s1));
    s1.sin_family = (short) AF_INET;
    s1.sin_addr.s_addr = htonl(INADDR_ANY);

    s1.sin_port = htons(0); /* bind() will give unique port */
    bind(listener, (struct sockaddr *)&s1, sizeof(s1));
    length = sizeof(s1);
    getsockname(listener, (struct sockaddr *)&s1, (socklen_t*) &length);
    char portBuff[256];
    sprintf(portBuff, "%d\n", ntohs(s1.sin_port));
    int portLength = strlen(portBuff);
    write(1, portBuff, strlen(portBuff));

    listen(listener, 1);

    length = sizeof(s2);

    conn = accept(listener, (struct sockaddr *)&s2, (socklen_t *) &length);
    if (close(conn) == -1)
        error("Could not close the connection!\n");

    return 0;
}