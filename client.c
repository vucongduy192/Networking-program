#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#include "config.h"
#include "GUI.h"
#include "queue.c"

GtkWidget *window, *box;
GtkWidget *label_name, *entry_name;
GtkWidget *label_room, *button_room[ROOM_NUM];
GtkWidget *label_wait, *msg_box, *scroll_window, *entry_msg;
int in_room = 0;
int client_sock = 0;
struct Queue *requests;

void screen_init();
void game_quit(GtkWidget *widget, gpointer *data);
void enter_name ();
void send_name (GtkWidget *widget, gpointer *data);
void choose_zoom ();
void send_room (GtkWidget *widget, gpointer *data);
void wait_room_friend();
void append_message(char *msg);
void send_msg_handler();

void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}
char * get_data(char command[]) {
	int i = 0, j;
	while (command[i] != ' ') {
		i++;
	}
	i++;

	char * data = malloc(LENGTH_MSG);
	for (j = 0; i+j < strlen(command); j++) {
		data[j] = command[i+j];
	}
	data[j] = '\0';
	return data;
}
void recv_msg_handler() {
	char *receive_message = malloc(LENGTH_MSG);
    while (1) {
		memset(receive_message, 0, strlen(receive_message)+1);
        int receive = recv(client_sock, receive_message, LENGTH_MSG, 0);
		
		if (receive > 0) {
			printf("\r%s\n", receive_message);
			if (strstr(receive_message, "new_client_success")) {
				choose_zoom(get_data(receive_message));
			}
			if (strstr(receive_message, "join_room_success")) {
				wait_room_friend(get_data(receive_message));
			}
			if (strstr(receive_message, "join_room_error")) {
				
			}
			if (msg_box)
				append_message(receive_message);
        } else if (receive == 0) {
            break;
        } else { 
            // -1 
        }
    }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(entry_msg));
    sprintf(message, "%s", send_buffer);
    send(client_sock, message, strlen(message), 0);
	puts(message);
	gtk_entry_set_text(GTK_ENTRY(entry_msg), "");
	append_message(message);
}

void screen_init() {   
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "destroy",
	       G_CALLBACK(game_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
	gtk_window_set_title(GTK_WINDOW(window), "Ai là triệu phú");
}

void game_quit(GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();
}

void enter_name () {
	box = gtk_vbox_new(0, 0);
	label_name = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_name), "<b>Enter your username</b>");
	gtk_container_add(GTK_CONTAINER(box), label_name);

	entry_name =  gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(box), entry_name);

	g_signal_connect(entry_name, "activate", G_CALLBACK(send_name), NULL);
		
	gtk_container_add(GTK_CONTAINER(window), box);
	gtk_widget_show_all(window);
}
void send_name(GtkWidget *widget, gpointer *data) {
	// Get username
	const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(widget));
	char new_client_cmd[LENGTH_MSG];
	sprintf(new_client_cmd, "./new_client %s", send_buffer);
	//send(client_sock, new_client_cmd, strlen(new_client_cmd),0);
	enQueue(requests, new_client_cmd);
	puts(new_client_cmd);
	choose_zoom();
}

Room room_arr[ROOM_NUM];
void convert_room_detail(char *data) {
	int i = 0, j, k = 0;
	char element[10];
	while(data[i]) {
		memset(element, 0, strlen(element));
		j = 0;
		while(data[i] != '-') {
			element[j++] = data[i++]; 
		}
		room_arr[k].id = atoi(element);
		memset(element, 0, strlen(element));
		j = 0;
		while(data[i] != '#') {
			element[j++] = data[i++]; 
		}
		room_arr[k].client_num = atoi(element);
		k++; // room index
		i++; // string index
	}
}
void choose_zoom(char *data) {
	gtk_widget_hide(label_name);
	gtk_widget_hide(entry_name);
	
	label_room = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_room), "<b>Choose a room</b>");
	gtk_container_add(GTK_CONTAINER(box), label_room);
	gtk_widget_show(label_room);

	convert_room_detail(data);
	char row[256];
	for(int i = 0; i < ROOM_NUM; i++) {
		memset(row, 0, strlen(row));
		sprintf(row, "Room %d - %d client\n", room_arr[i].id, room_arr[i].client_num);
		button_room[i] = gtk_button_new_with_label(row);
		gtk_container_add(GTK_CONTAINER(box), button_room[i]);
		int *room_id= malloc(sizeof(int));
		*room_id = i+1;
		g_signal_connect(G_OBJECT(button_room[i]), "clicked", G_CALLBACK(send_room), (gpointer) room_id);
		gtk_widget_show(button_room[i]);
	}
	gtk_widget_show(window);
}
void send_room(GtkWidget *widget, gpointer *data) {
	//printf("\r%d", *((int *)data));
	
	char join_room_cmd[LENGTH_MSG];
	sprintf(join_room_cmd, "./join_room %d", *((int *)data));
	//send(client_sock, join_room_cmd, strlen(join_room_cmd),0);
	puts(join_room_cmd);
}
void wait_room_friend(char *data) {
	for (int i = 0; i < ROOM_NUM; i++)
		gtk_widget_hide(button_room[i]);
	gtk_widget_hide(label_room);

	printf("%s",data);
	label_wait = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_wait), "<b>Waiting another client</b>");
	gtk_container_add(GTK_CONTAINER(box), label_wait);
	gtk_widget_show(label_wait);
	
	msg_box = gtk_text_view_new();
	//scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_box_pack_start(GTK_BOX(box), msg_box, TRUE, TRUE, 0);
	//gtk_container_add (GTK_CONTAINER (scroll_window), msg_box);

	// gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_window),
	// 								GTK_POLICY_AUTOMATIC,
	// 								GTK_POLICY_AUTOMATIC);

	//gtk_box_pack_start (GTK_BOX (box), scroll_window, TRUE, TRUE, 0);
	gtk_widget_show(msg_box);
	//gtk_widget_show(scroll_window);

	entry_msg = gtk_entry_new();
	g_signal_connect(entry_msg, "activate", G_CALLBACK(send_msg_handler), NULL);
	gtk_container_add(GTK_CONTAINER(box), entry_msg);
	gtk_widget_show(entry_msg);
	gtk_widget_show(window);
}

void append_message(char *msg) {
	//msg[strlen(msg)]='\n';msg[strlen(msg)]='\0';
	GtkTextBuffer * buffer;
	GtkTextMark * mark;
	GtkTextIter iter;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msg_box));

	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	if (gtk_text_buffer_get_char_count(buffer))
		gtk_text_buffer_insert(buffer, &iter, "\n", 1);

	gtk_text_buffer_insert(buffer, &iter, msg, -1);
	// gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(msg_box), mark);
}

gboolean timer_exe(gpointer p)
{
    char msg[1024];
	struct QNode * req = deQueue(requests);
    if (req) {
		strcpy(msg, req->key);
		send(client_sock, msg, strlen(msg), 0);
	}
    return TRUE;
}
int main(int argc, char *argv[]){
	requests = createQueue(); 

	if (!g_thread_supported ()){ g_thread_init(NULL); }
	// initialize GDK thread support
	gdk_threads_init();
	gdk_threads_enter();
	gtk_init(&argc, &argv);

	struct sockaddr_in server_socket;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(SERVER_PORT);
    server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("server IP = %s ", inet_ntoa(server_socket.sin_addr));

    if (connect(client_sock, (struct sockaddr *)&server_socket, sizeof(server_socket)) < 0)
        printf("Error in connecting to server\n");
    else
        printf("connected to server\n");

    // Signal driven I/O mode and NONBlOCK mode so that recv will not block
    if (fcntl(client_sock, F_SETFL, O_NONBLOCK | O_ASYNC))
        printf("Error in setting socket to async, nonblock mode");

    signal(SIGIO, recv_msg_handler); // assign SIGIO to the handler
    // set this process to be the process owner for SIGIO signal
    if (fcntl(client_sock, F_SETOWN, getpid()) < 0)
        printf("Error in setting own to socket");
	g_timeout_add(100, (GSourceFunc)timer_exe, NULL);
	screen_init(); 
	enter_name();
	gtk_main();
    gdk_threads_leave();
	close(client_sock);
	return 0;
}
