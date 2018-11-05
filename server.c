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

void client_handler(void *p_client) {
        char recv_buffer[LENGTH_MSG] = {};
        char send_buffer[LENGTH_SEND] = {};
        
        int client_id = *((int *)p_client);
        // Conversation
         while (1) {
                int receive = recv(client_id, recv_buffer, LENGTH_MSG, 0);
                if (receive > 0) {
                        if (strlen(recv_buffer) == 0) {
                                continue;
                        }
                        sprintf(send_buffer, "%d：%s",  client_id, recv_buffer);
                } else if (receive == 0 || strcmp(recv_buffer, "exit") == 0) {
                        printf("%d leave the chatroom.\n", client_id);
                        sprintf(send_buffer, "%d leave the chatroom.", client_id);
                } else {
                        printf("Fatal Error: -1\n");
                }
        }
}

int main() {
        int server_sockfd = 0, client_sockfd = 0;

        // Create socket
        server_sockfd = socket(AF_INET , SOCK_STREAM , 0);
        if (server_sockfd == -1) {
                printf("Fail to create a socket.");
                exit(EXIT_FAILURE);
        }

        // Socket information
        struct sockaddr_in server_info, client_info;
        int s_addrlen = sizeof(server_info);
        int c_addrlen = sizeof(client_info);
        memset(&server_info, 0, s_addrlen);
        memset(&client_info, 0, c_addrlen);
        server_info.sin_family = AF_INET;
        server_info.sin_addr.s_addr = INADDR_ANY;
        server_info.sin_port = htons(8888);
        
        // Bind and Listen
        bind(server_sockfd, (struct sockaddr *)&server_info, s_addrlen);
        listen(server_sockfd, 5);

        // Print Server IP
        getsockname(server_sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
        printf("Start Server on: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));



        while (1) {
                client_sockfd = accept(server_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);

                // Print Client IP
                getpeername(client_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
                printf("Client %s:%d come in.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

                int * cur = (int *)malloc(sizeof(int));
                *cur = client_sockfd;

                pthread_t id;
                if (pthread_create(&id, NULL, (void *)client_handler, (void *)cur) != 0) {
                        perror("Create pthread error!\n");
                        exit(EXIT_FAILURE);
                }
        }




        return 0;
}