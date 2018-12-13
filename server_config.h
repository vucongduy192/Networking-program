#define PORT 8888
#define CLIENT_MAX 20   /* Number of allowed connections */

#define LENGTH_MSG 1024
#define BUFF_SIZE 1024

#define ROOM_SIZE 2
#define ROOM_MAX 6
 
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

typedef struct Answer {
    int q_num;
    int q_option;
} Answer;
