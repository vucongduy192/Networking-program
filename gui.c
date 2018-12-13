
GtkWidget *window, *box, *table;        
GtkWidget *label_name, *entry_name;
GtkWidget *label_room, *button_room[ROOM_NUM];
GtkWidget *label_wait, *msg_box = NULL, *scroll_window, *entry_msg, *friend_box, *label_client[ROOM_SIZE], *box_client;
GtkWidget *label_start, *label_question, *button_option[4], *text_view_question, *label_ring;
 
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
void send_answer (GtkWidget *widget, gpointer *data);
/* --------------------------------*/

void game_init() {   
	 window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window), "destroy",
	       G_CALLBACK(game_quit), NULL);
	gtk_window_set_default_size(GTK_WINDOW(window), SCREEN_WIDTH, SCREEN_HEIGHT);
	gtk_window_set_title(GTK_WINDOW(window), "Ai là triệu phú");
}

void enter_name_screen () {
	//box = gtk_vbox_new(0, 0);
	table = gtk_table_new(7, 4, TRUE);
	label_name = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_name), "<b>Enter your username</b>");
	//gtk_container_add(GTK_CONTAINER(box), label_name);
	gtk_table_attach_defaults(GTK_TABLE(table), label_name, 1, 3, 1, 2);
	// gtk_widget_set_size_request((text_view_question), 400, 200);
	entry_name =  gtk_entry_new();
	//gtk_container_add(GTK_CONTAINER(box), entry_name);
	gtk_table_attach_defaults(GTK_TABLE(table), entry_name, 1, 3, 2, 3);

	g_signal_connect(entry_name, "activate", G_CALLBACK(send_name), NULL);
	
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show_all(window);
}

void choose_zoom_screen(char *data) {
	puts("Choose room");
	gtk_widget_hide(label_name);
	gtk_widget_hide(entry_name);
	
	label_room = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label_room), "<b>Choose a room</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_room, 1, 3, 0, 1);
	gtk_widget_show(label_room);

	convert_room_detail(data);
	char row[256];
	for(int i = 0; i < ROOM_NUM; i++) {
		memset(row, 0, strlen(row));
		sprintf(row, "Room %d - %d client\n", room_arr[i].id, room_arr[i].client_num);
		button_room[i] = gtk_button_new_with_label(row);
		
		gtk_table_attach_defaults(GTK_TABLE(table), button_room[i], 1, 3, i+1, i+2);
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
	gtk_label_set_markup(GTK_LABEL(label_wait), "<b>Waiting another client</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_wait, 2, 4, 0, 1);
	gtk_widget_show(label_wait);
	refresh_friend_room(data);
	
	msg_box = gtk_text_view_new();
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scroll_window), msg_box);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_window),
									GTK_POLICY_AUTOMATIC,
									GTK_POLICY_AUTOMATIC);

	//gtk_box_pack_start (GTK_BOX (box), scroll_window, TRUE, TRUE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), scroll_window, 2, 4, 2, 5);
	gtk_widget_show(msg_box);
	gtk_widget_show(scroll_window);

	entry_msg = gtk_entry_new();
	g_signal_connect(entry_msg, "activate", G_CALLBACK(send_msg), NULL);
	//gtk_container_add(GTK_CONTAINER(box), entry_msg);
	gtk_table_attach_defaults(GTK_TABLE(table), entry_msg, 2, 4, 5, 6);
	gtk_widget_show(entry_msg);
	gtk_widget_show(window);
}

void refresh_friend_room(data) {
	convert_client_detail(data);
	for(int i = 0; i < client_num; i++) {
		puts(client_arr[i].name);
		if (label_client[i] == NULL) {
			label_client[i] = gtk_label_new(NULL);
			gtk_label_set_text(GTK_LABEL(label_client[i]), client_arr[i].name);
			//gtk_box_pack_end(GTK_BOX(box_client), label_client[i], TRUE, TRUE, 5);
			gtk_table_attach_defaults(GTK_TABLE(table), label_client[i], 2+i, 3+i, 1, 2);
			gtk_widget_show(label_client[i]);
		} else gtk_label_set_text(GTK_LABEL(label_client[i]), client_arr[i].name);
	}
	if(client_num == ROOM_SIZE) {
		label_start = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(label_start), "<b>Let's start</b>");
		gtk_table_attach_defaults(GTK_TABLE(table), label_start, 2, 4, 0, 1);
		gtk_widget_show(label_start);
		play_game(); 
	}
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

void new_question() {
	if (label_question == NULL) {
		label_question = gtk_label_new(NULL);
		gtk_table_attach_defaults(GTK_TABLE(table), label_question, 0, 2, 0, 4);
		gtk_widget_show(label_question);
	}
	char temp[1024];
	sprintf(temp, "%d : %s", q_cur+1, q_arr[q_cur].question);
	gtk_label_set_text(GTK_LABEL(label_question), temp);
	for (int i = 0; i < 4; i++) {
		if (button_option[i] != NULL) {
			gtk_button_set_label(GTK_BUTTON(button_option[i]), q_arr[q_cur].option[i]);
			continue;
		}
		button_option[i] = gtk_button_new_with_label(q_arr[q_cur].option[i]);
		gtk_table_attach_defaults(GTK_TABLE(table), button_option[i], i%2, 1+i%2, 4+i/2, 5+i/2);
		int * a = malloc(sizeof(int));
		*a = i;
		g_signal_connect(G_OBJECT(button_option[i]), "clicked", G_CALLBACK(send_answer), (gpointer) a);
		gtk_widget_show(button_option[i]);
	}
}

void play_game() {
	running = TRUE;
	q_cur = 0;
	load_question();
	new_question();
	gtk_widget_hide(label_wait);
}

void game_quit(GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();
}