int client_sock = 0, client_num = 0;
struct Queue *responses;
Room room_arr[ROOM_NUM];
Client client_arr[ROOM_SIZE];

GtkWidget *window, *box;        
GtkWidget *label_name, *entry_name;
GtkWidget *label_room, *button_room[ROOM_NUM];
GtkWidget *label_wait, *msg_box = NULL, *scroll_window, *entry_msg, *friend_box, *label_client[ROOM_SIZE], *box_client;

void game_init();
void game_quit(GtkWidget *widget, gpointer *data);
void enter_name_screen();
void choose_zoom_screen ();
void wait_friend_screen();
void append_message(char *msg);
/* ----- Send request function ----*/
void send_name (GtkWidget *widget, gpointer *data);
void send_room (GtkWidget *widget, gpointer *data);
void send_msg();

void game_init() {   
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "destroy",
	       G_CALLBACK(game_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
	gtk_window_set_default_size(GTK_WINDOW(window), SCREEN_WIDTH, SCREEN_HEIGHT);
	gtk_window_set_title(GTK_WINDOW(window), "Ai là triệu phú");
}

void enter_name_screen () {
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

void choose_zoom_screen(char *data) {
	puts("Choose room");
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

void wait_friend_screen(char *data) {
	for (int i = 0; i < ROOM_NUM; i++)
		gtk_widget_hide(button_room[i]);
	gtk_widget_hide(label_room);

	label_wait = gtk_label_new(NULL);
	gtk_widget_set_size_request (label_wait, SCREEN_MSG_WIDTH, 1);
	gtk_label_set_markup(GTK_LABEL(label_wait), "<b>Waiting another client</b>");

	gtk_container_add(GTK_CONTAINER(box), label_wait);
	gtk_widget_show(label_wait);
	refresh_friend_room(data);
	
	msg_box = gtk_text_view_new();
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scroll_window), msg_box);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_window),
									GTK_POLICY_AUTOMATIC,
									GTK_POLICY_AUTOMATIC);

	gtk_box_pack_start (GTK_BOX (box), scroll_window, TRUE, TRUE, 0);
	gtk_widget_show(msg_box);
	gtk_widget_show(scroll_window);

	entry_msg = gtk_entry_new();
	g_signal_connect(entry_msg, "activate", G_CALLBACK(send_msg), NULL);
	gtk_container_add(GTK_CONTAINER(box), entry_msg);
	gtk_widget_show(entry_msg);
	gtk_widget_show(window);
}

void refresh_friend_room(data) {
	convert_client_detail(data);
	if (box_client == NULL) {
		box_client = gtk_hbox_new(0, 0);
		gtk_widget_set_size_request (box_client, SCREEN_MSG_WIDTH, 50);
		gtk_container_add(GTK_CONTAINER(box), box_client);
		gtk_widget_show(box_client);
	}
	for(int i = 0; i < client_num; i++) {
		if (label_client[i] == NULL) {
			label_client[i] = gtk_label_new(NULL);
			gtk_label_set_text(GTK_LABEL(label_client[i]), g_locale_to_utf8 (client_arr[i].name,-1,0,0,0));
			gtk_box_pack_end(GTK_BOX(box_client), label_client[i], TRUE, TRUE, 5);
			gtk_widget_show(label_client[i]);
		} else gtk_label_set_text(GTK_LABEL(label_client[i]), g_locale_to_utf8 (client_arr[i].name,-1,0,0,0));
	}
	if(client_num == 3)
		play_game(); 
}

void append_message(char *msg) {
	GtkTextBuffer * buffer;
	GtkTextMark * mark;
	GtkTextIter iter;
	if (msg_box == NULL)
		return;
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msg_box));

	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	if (gtk_text_buffer_get_char_count(buffer))
		gtk_text_buffer_insert(buffer, &iter, "\n", 1);

	gtk_text_buffer_insert(buffer, &iter, msg, -1);
	gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(msg_box), mark);
}

void play_game() {
	gtk_label_set_text(GTK_LABEL(label_wait), g_locale_to_utf8 ("Let's start", -1,0,0,0));
	
}


void game_quit(GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();
}