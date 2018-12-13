#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888

#define LENGTH_MSG 1024
#define BUFF_SIZE 1024

#define ROOM_SIZE 2
#define ROOM_NUM 6
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 800
#define NEW_CLIENT_SUCESS "new_client_success"
#define NEW_CLIENT_ERROR "new_client_error"

#define TRUE 1
#define FALSE 0 

typedef struct Client {
    int connfd;
    int room_id;
    char name[100];
} Client;

typedef struct Room {
    int id;
    int client_num;
} Room;

typedef struct Answer {
    int q_num;
    int q_option;
} Answer;

int client_sock = 0, client_num = 0;
struct Queue *responses;
Room room_arr[ROOM_NUM];
Client client_arr[ROOM_SIZE];
int running = FALSE;
int q_cur;