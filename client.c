#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
<<<<<<< HEAD:client.c
#include <SDL2/SDL.h>

#include "config.h"

static const int width = 800;
static const int height = 600;

int client_sock = 0;
int flag = 0;
void str_trim_lf(char * arr, int length) {
=======


#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 1024

#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201
int client_sock = 0;
void str_trim_lf (char* arr, int length) {
>>>>>>> parent of faec78a... add new room_chat:tcp_client.c
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

void recv_msg_handler() {
    char receiveMessage[LENGTH_SEND] = {};
    while (1) {
        int receive = recv(client_sock, receiveMessage, LENGTH_SEND, 0);
        if (receive > 0) {
            printf("\r%s\n", receiveMessage);
<<<<<<< HEAD:client.c
            memset(receiveMessage, 0, strlen(receiveMessage) + 1);
            wait_msg();
=======
            str_overwrite_stdout();
>>>>>>> parent of faec78a... add new room_chat:tcp_client.c
        } else if (receive == 0) {
            break;
        } else {
            // -1 
        }
    }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    while (1) {
        str_overwrite_stdout();
        while (fgets(message, LENGTH_MSG, stdin) != NULL) {
            str_trim_lf(message, LENGTH_MSG);
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }
        send(client_sock, message, LENGTH_MSG, 0);
        if (strcmp(message, "exit") == 0) {
            break;
        }
    }
}

<<<<<<< HEAD:client.c
int main() {
    struct sockaddr_in server_addr; /* server's address information */
    //Step 1: Construct socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    //Step 2: Specify server address
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    //Step 3: Request to connect server
    if (connect(client_sock, (struct sockaddr * ) & server_addr, sizeof(struct sockaddr)) < 0) {
        printf("\nError!Can not connect to sever! Client exit imediately! ");
        return 0;
    }

    //Step 4: Communicate with server
    int room_id;           
    char name[LENGTH_NAME];
    printf("Enter your name : ");
    fgets(name, LENGTH_NAME, stdin);
    printf("Enter room : ");
    scanf("%d", &room_id);
    
    send(client_sock, name, LENGTH_NAME, 0);
=======
int main(){
	char buff[BUFF_SIZE + 1];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;
	
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
		
	//Step 4: Communicate with server			
>>>>>>> parent of faec78a... add new room_chat:tcp_client.c

    pthread_t send_msg_thread;
    if (pthread_create( & send_msg_thread, NULL, (void * ) send_msg_handler, NULL) != 0) {
        printf("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_msg_thread;
    if (pthread_create( & recv_msg_thread, NULL, (void * ) recv_msg_handler, NULL) != 0) {
        printf("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

<<<<<<< HEAD:client.c
    close(client_sock);
    return 0;
}
=======
    while (1) {
    }
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}
>>>>>>> parent of faec78a... add new room_chat:tcp_client.c
