#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include "error.h"

// Create a server which can obtain a TCP port and print the port number on STD OUT
struct sockaddr_in *s_in; char* s1, * s2;


int main(int argc, char** argv) {
    int listener, conn, length; char ch;

    conn = -1;

    int connections[32], state[32], position[32];
    for (int i = 0; i < 32; i++) {
        connections[i]=-1;
        state[i] = 0;
        position[i] = 0;
    }



    struct sockaddr_in s1, s2;

    listener = socket( AF_INET, SOCK_STREAM, 0);

    bzero((char *) &s1, sizeof(s1));
    s1.sin_family = (short) AF_INET;
    s1.sin_addr.s_addr = htonl(INADDR_ANY);

    s1.sin_port = htons(0); /* bind() will give unique port */
    bind(listener, (struct sockaddr *)&s1, sizeof(s1));
    length = sizeof(s1);
    getsockname(listener, (struct sockaddr *)&s1, (socklen_t*) &length);
    char portBuff[128];
    sprintf(portBuff, "%d\n", ntohs(s1.sin_port));
    int portLength = strlen(portBuff);
    write(1, portBuff, strlen(portBuff));

    listen(listener, 1);

    length = sizeof(s2);
    
    fd_set readfds;
    struct timeval timeout;
    int maxVal;
    int selection;
    int readChar;
    char buff[128];

    while (1) {
        maxVal = -1;
        readChar = 0;
        FD_ZERO(&readfds);

        FD_SET(listener, &readfds);
        FD_SET(0, &readfds);
        for (int i = 0; i < 32; i++) {
            if (connections[i] > -1)
                FD_SET(connections[i], &readfds);
        }

        timeout.tv_sec = 300;
        timeout.tv_usec = 0;

        if (listener > maxVal)
            maxVal = listener;

        for (int i = 0; i < 32; i++)
            if (connections[i] != -1 && connections[i] > maxVal)
                maxVal = connections[i];


        selection = select(maxVal + 1, &readfds, NULL, NULL, &timeout);

        switch(selection) {
            case (-1):
                error("Error: Error during selection...\n");
                break;


            case (0):
                if (conn > -1) 
                    close(conn);
                
                return 0;
                break;

            default:
                if(FD_ISSET(listener, &readfds)) {
                    for (int i = 0; i < 32; i++) {
                        if (connections[i] == -1) {
                            connections[i] = accept(listener, (struct sockaddr *)&s2, (socklen_t *) &length);
                            state[i] = 1;
                            position[i] = 0;
                            write(connections[i], "Log #: ", 6);
                            break;
                        }
                    }
                }

                else if(FD_ISSET(0, &readfds)) {
                    readChar = read(0, &buff, 128);
                    if (readChar == -1) 
                        error("Error: Could not read\n");
                    
                    write(1, &buff, readChar);
                }

                for (int i = 0; i < 32; i++) {
                    if(connections[i] != -1 && FD_ISSET(connections[i], &readfds)) {
                        readChar = read(connections[i], &buff, 128);
                        if (readChar == -1) 
                            error("Error: Could not read\n");

                        else if(readChar == 0) {
                            close(connections[i]);
                            state[i] = 0;
                            connections[i] = -1;
                            break;
                        }
                    
                        write(1, &buff, readChar);
                    }
                }
                break;
        }
    }

    return 0;
}