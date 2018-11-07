#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "config.h"

Room room_arr[ROOM_NUM];
int room_num = 0;

Client client_arr[CLIENT_NUM];
int client_num = 0;
struct sockaddr_in server, client;

// Remove char in string form index to end 
void str_trim(char *str, int index) {
	for (int i=strlen(str)-1; i>=index; i--) {
		str[i] = '\0';
	}
}

Client get_client(int connfd) {
	for (int i =0; i < client_num; i++) {
		if (client_arr[i].connfd == connfd)
			return client_arr[i];
	}
}

void new_client(int connfd, char *buff, int room_id) {
	for (int i = 0; i < client_num; i++) {
		if (client_arr[i].connfd == connfd) {
			return;
		}
	}
	str_trim(buff, strlen(buff)-1);
	client_arr[client_num].connfd = connfd;
	client_arr[client_num].room_id = room_id;
	strcpy(client_arr[client_num++].name, buff);
}

void send_msg_room(int connfd, int room_id, char *buff) {
	for (int i = 0; i < client_num; i++) {
		if (client_arr[i].connfd != connfd && client_arr[i].room_id == room_id) {
			char msg[LENGTH_MSG];
			sprintf(msg,"%s : %s", get_client(connfd).name, buff);
			send(client_arr[i].connfd, msg, strlen(msg), 0);
		}
	}
}
void *echo(void *arg){
	int bytes_sent, bytes_received;
	char buff[BUFF_SIZE + 1];
	int connfd = *((int *) arg);
	
	pthread_detach(pthread_self());
	bytes_received = recv(connfd, buff, BUFF_SIZE, 0); //Name of client
	int room_id = client_num/ROOM_SIZE + 1;
	room_arr[room_num].id = room_id;

	new_client(connfd, buff, room_id);

	printf("You got a connection from %s - Port : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port)); /* prints client's IP */
	printf("%s join room %d\n", buff, room_id);	
	// Notify to other member in room
	sprintf(buff, "join room %d", room_id);
	send_msg_room(connfd, room_id, buff);
	
	// Join room
	sprintf(buff, "------------------Room %d------------------", room_id);
	send(connfd, buff, strlen(buff), 0);	
	
	// Game start
	
	while (1) {
		bytes_received = recv(connfd, buff, BUFF_SIZE, 0); //blocking
		if (bytes_received < 0) {
			perror("\nError: ");
			break;
		}
		else if (bytes_received == 0) {
			printf("Connection closed.");
			break;
		}
		send_msg_room(connfd, room_id, buff);
	}
	
}

int main()
{ 
	int listenfd, connfd;
	pthread_t tid;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(SERVER_PORT); 
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	
	if(bind(listenfd,(struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listenfd, CLIENT_NUM) == -1){  
		perror("\nError: ");
		return 0;
	}
	printf("Start Server on: %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
	int client_size = sizeof(struct sockaddr_in);
	while(1){		
		if ((connfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t*)&client_size)) ==- 1)
			perror("\nError: ");
		pthread_create(&tid, NULL, &echo, &connfd);	
	}
	
	close(listenfd);
	return 0;
}