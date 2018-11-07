#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define CLIENT_NUM 20   /* Number of allowed connections */

#define LENGTH_NAME 31
#define LENGTH_MSG 101
#define LENGTH_SEND 201
#define BUFF_SIZE 1024

#define ROOM_SIZE 3 
#define ROOM_NUM 5 

typedef struct Client {
    int connfd;
	int room_id;
    char name[31];
} Client;

typedef struct Room {
    int id;
    int client_num;
} Room;