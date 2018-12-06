#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888

#define LENGTH_MSG 1024
#define BUFF_SIZE 1024

#define ROOM_SIZE 3 
#define ROOM_NUM 5
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 800
#define SCREEN_MSG_WIDTH 600
#define NEW_CLIENT_SUCESS "new_client_success"
#define NEW_CLIENT_ERROR "new_client_error"

typedef struct Client {
    int connfd;
    int room_id;
    char name[100];
} Client;

typedef struct Room {
    int id;
    int client_num;
} Room;
