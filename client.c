#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define LENGTH_MSG 101
#define LENGTH_SEND 201

int sockfd = 0;

void recv_msg_handler() {
        printf("thread recv\n");
        char receiveMessage[LENGTH_SEND] = {};
        while (1) {
                int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);
                if (receive > 0) {
                        printf("\r%s\n", receiveMessage);
                }
        }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    while (1) {
        while (fgets(message, LENGTH_MSG, stdin) != NULL);
        printf("%s\n", message);
        send(sockfd, message, LENGTH_MSG, 0);
    }
}

int main (){

        // Create socket
        sockfd = socket(AF_INET , SOCK_STREAM , 0);
        if (sockfd == -1) {
            printf("Fail to create a socket.");
            exit(EXIT_FAILURE);
        }

        // Socket information
        struct sockaddr_in server_info, client_info;
        int s_addrlen = sizeof(server_info);
        int c_addrlen = sizeof(client_info);
        memset(&server_info, 0, s_addrlen);
        memset(&client_info, 0, c_addrlen);
        server_info.sin_family = PF_INET;
        server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_info.sin_port = htons(8888);

        // Connect to Server
        int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
        if (err == -1) {
                printf("Connection to Server error!\n");
                exit(EXIT_FAILURE);
        }
        send(sockfd, "Client", 6, 0);

        pthread_t send_msg_thread;
        if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
                printf ("Create pthread error!\n");
                exit(EXIT_FAILURE);
        }

        pthread_t recv_msg_thread;
        if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
                printf ("Create pthread error!\n");
                exit(EXIT_FAILURE);
        }
        while (1) {}
        return 0;
}