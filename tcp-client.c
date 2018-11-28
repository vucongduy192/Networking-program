#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>

#include "config.h"
int client_sock_fd;
int main(int argc, char const *argv[])
{
    struct sockaddr_in server_socket;
    client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(SERV_PORT);
    server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("server IP = %s ", inet_ntoa(server_socket.sin_addr));

    if (connect(client_sock_fd, (struct sockaddr *)&server_socket, sizeof(server_socket)) < 0)
        printf("Error in connecting to server\n");
    else
        printf("connected to server\n");

    // Signal driven I/O mode and NONBlOCK mode so that recv will not block
    if (fcntl(client_sock_fd, F_SETFL, O_NONBLOCK | O_ASYNC))
        // if (fcntl(client_sock_fd, F_SETFL, O_NONBLOCK))
        printf("Error in setting socket to async, nonblock mode");

    //signal(SIGIO, signio_handler); // assign SIGIO to the handler
    //signal(SIGABRT, signal_SIGABRT);
    // set this process to be the process owner for SIGIO signal
    if (fcntl(client_sock_fd, F_SETOWN, getpid()) < 0)
        printf("Error in setting own to socket");
	return 0;
}
