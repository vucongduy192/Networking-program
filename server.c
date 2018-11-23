#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 20   /* Number of allowed connections */
#define BUFF_SIZE 1024

#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201

/* Receive and echo message to client */
void *echo(void *);
int connfd_arr[20];
int connfd_length = 0;

void push_connfd(int *connfd) {
	for (int i = 0; i < connfd_length; i++) {
		if (connfd_arr[i] == *connfd) {
			return;
		}
	}
	connfd_arr[connfd_length++] = *connfd;
}
int main()
{ 
	int listenfd, *connfd;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in *client; /* client's address information */
	int sin_size;
	pthread_t tid;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT); 
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   

	if(bind(listenfd,(struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listenfd, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}
	
	sin_size=sizeof(struct sockaddr_in);
	client = malloc(sin_size);
	while(1){		
		connfd = malloc(sizeof(int));
		if ((*connfd = accept(listenfd, (struct sockaddr *)client, &sin_size)) ==- 1)
			perror("\nError: ");
		push_connfd(connfd);
		printf("You got a connection from %s\n", inet_ntoa(client->sin_addr) ); /* prints client's IP */
		
		/* For each client, spawns a thread, and the thread handles the new client */
		pthread_create(&tid, NULL, &echo, connfd);	
	}
	
	close(listenfd);
	return 0;
}

void *echo(void *arg){
	int connfd;
	int bytes_sent, bytes_received;
	char buff[BUFF_SIZE + 1];

	connfd = *((int *) arg);
	free(arg);
	pthread_detach(pthread_self());
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
		printf("From %d : %s\n", connfd, buff);
		bytes_sent = send(connfd, buff, bytes_received, 0); /* send to the client welcome message */
		for (int i = 0; i < connfd_length; i++) {
			if (connfd_arr[i] != connfd)
				send(connfd_arr[i], buff, bytes_received, 0);
		}
		if (bytes_sent < 0){
			perror("\nError: ");
			close(connfd);
			break;	
		}
	}
	
}
