#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888

#define LENGTH_MSG 512
#define BUFF_SIZE 512

#define ROOM_SIZE 2
#define ROOM_NUM 6
#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 800
#define NEW_CLIENT_SUCESS "new_client_success"
#define NEW_CLIENT_ERROR "new_client_error"

#define TRUE 1
#define FALSE 0 
#define MODAL_YES -8
#define MODAL_NO -9
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

int client_sock = 0, running_client = 0;
struct Queue *responses;
Room room_arr[ROOM_NUM];
Client client_arr[ROOM_SIZE];
int running = FALSE;
int in_choose_room = FALSE;
int in_waiting_friend = FALSE;
int q_cur;
char client_name[LENGTH_MSG];
int room_id;

#define LEFT_ROOM_MODAL "Bạn chắc chắn muốn rời phòng chơi"
#define ELIMINATED "Bạn đã bị loại"

// GUI
GtkWidget *window, *box, *table;        
GtkWidget *label_name, *entry_name;
GtkWidget *label_room = NULL, *button_room[ROOM_NUM];
GtkWidget *label_wait, *btn_back, *msg_box = NULL, *scroll_window, *entry_msg, *friend_box, *label_client[ROOM_SIZE], *box_client;
GtkWidget *label_start, *label_question, *button_option[4], *text_view_question, *label_ring;
GtkWidget *end_game_result;