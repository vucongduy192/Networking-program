 
void game_init();
void game_quit(GtkWidget *widget, gpointer *data);
void enter_name_screen();
void choose_zoom_screen ();
void wait_friend_screen();
void append_message(char *msg);
void enter_name_clear();
void choose_zoom_clear();
void in_game_clear();
void win_game_screen();
void result_screen();
void lose_game_screen();
void enter_name_clear();
void end_game_clear();
/* ----- Send request function ----*/
void send_name (GtkWidget *widget, gpointer *data);
void send_room (GtkWidget *widget, gpointer *data);
void send_msg();
void send_answer (GtkWidget *widget, gpointer *data);
void send_back();
/* --------------------------------*/
GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

void set_background(const gchar * filename) {
	image = create_pixbuf (filename);
	gdk_pixbuf_render_pixmap_and_mask (image, &background, NULL, 0);
    style = gtk_style_new ();
    style->bg_pixmap [0] = background;
	gtk_widget_set_style (GTK_WIDGET(window), GTK_STYLE (style));
}

void game_init() {   

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	GdkPixbuf *icon = create_pixbuf("images/logo.png");  
  	gtk_window_set_icon(GTK_WINDOW(window), icon); 
	g_object_unref(icon);    
	set_background("images/enter_name.jpg");

	g_signal_connect(G_OBJECT(window), "destroy",
	       G_CALLBACK(game_quit), NULL);
	gtk_window_set_default_size(GTK_WINDOW(window), SCREEN_WIDTH, SCREEN_HEIGHT);
	gtk_window_set_title(GTK_WINDOW(window), "Ai là triệu phú");
}

void enter_name_screen () {
	table = gtk_table_new(7, 4, TRUE);
	
	label_name = gtk_label_new(NULL);
	// gtk_label_set_markup(GTK_LABEL(label_name), "<b>Enter your username</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), label_name, 1, 3, 1, 2);
	
	entry_name =  gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), entry_name, 1, 3, 2, 3);

	g_signal_connect(entry_name, "activate", G_CALLBACK(send_name), NULL);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show_all(window);
}

void enter_name_clear () {
	if (label_name != NULL) {
		gtk_widget_hide(label_name);
		gtk_widget_hide(entry_name);
	}
}

void choose_zoom_screen(char *data) {
	enter_name_clear();
	in_game_clear();
	end_game_clear();
	set_background("images/in_game.png");

	in_choose_room = TRUE;
	in_waiting_friend = FALSE;
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

void choose_zoom_clear() {
	in_choose_room = FALSE;
	for (int i = 0; i < ROOM_NUM; i++)
		gtk_widget_hide(button_room[i]);
	gtk_widget_hide(label_room);
	label_room = NULL;
}

void wait_friend_screen(char *data) {
	choose_zoom_clear();
	step = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(step), "Nhiệm vụ : vượt qua 5 câu hỏi");
	gtk_table_attach_defaults(GTK_TABLE(table), step, 2, 4, 0, 1);
	gtk_widget_show(step);

	in_waiting_friend = TRUE;
	refresh_friend_room(data);
	if (running_client <= ROOM_SIZE - 1) 
		show_info("Please wait another player!");

	btn_back = gtk_button_new_with_label("Back");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_back, 0, 1, 0, 1);
	g_signal_connect(btn_back, "clicked", G_CALLBACK(show_question), NULL);
	gtk_widget_show(btn_back);

	msg_box = gtk_text_view_new();
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scroll_window), msg_box);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_window),
									GTK_POLICY_AUTOMATIC,
									GTK_POLICY_AUTOMATIC);

	gtk_table_attach_defaults(GTK_TABLE(table), scroll_window, 2, 4, 2, 5);
	gtk_widget_show(msg_box);
	gtk_widget_show(scroll_window);

	entry_msg = gtk_entry_new();
	g_signal_connect(entry_msg, "activate", G_CALLBACK(send_msg), NULL);
	gtk_table_attach_defaults(GTK_TABLE(table), entry_msg, 2, 4, 5, 6);
	gtk_widget_show(entry_msg);
	gtk_widget_show(window);
}



void refresh_friend_room(data) {
	for(int i = 0; i < ROOM_SIZE; i++) {
		if (label_client[i] != NULL) {
			gtk_widget_hide(label_client[i]);
			label_client[i] = NULL;
		}
	}
	convert_client_detail(data);
	
	for(int i = 0; i < running_client; i++) {
		puts(client_arr[i].name);
		if (label_client[i] == NULL) {
			label_client[i] = gtk_label_new(NULL);
			// Make client_name to bold if current client
			char temp[LENGTH_MSG];
			if (strcmp(client_name, client_arr[i].name) == 0) {
				sprintf(temp,"<b>%s</b>", client_arr[i].name);
				gtk_label_set_markup(GTK_LABEL(label_client[i]), temp);
			}
			else gtk_label_set_text(GTK_LABEL(label_client[i]), client_arr[i].name);
			gtk_table_attach_defaults(GTK_TABLE(table), label_client[i], 2+i, 3+i, 1, 2);
			gtk_widget_show(label_client[i]);
		} 
	}
	if(running_client == ROOM_SIZE) {
		show_info("Phòng chơi đã đầy. Bắt đầu ngay!");
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

void in_game_clear() {
	if (btn_back != NULL) {
		// Chat area
		for(int i = 0; i < ROOM_SIZE; i++) {
			if (label_client[i] != NULL) {
				gtk_widget_hide(label_client[i]);
				label_client[i] = NULL;
			}
		}
		gtk_widget_hide(btn_back);
		gtk_widget_hide(entry_msg);
		gtk_widget_hide(msg_box);
		gtk_widget_hide(step);
		msg_box = NULL;
	}
	if (label_question != NULL) {
		// Answer question area
		gtk_widget_hide(label_question);
		label_question = NULL;
		for (int i = 0; i < 4; i++) {
			gtk_widget_hide(button_option[i]);
			button_option[i] = NULL;
		}
	}
}

void play_game() {
	running = TRUE;
	q_cur = 0;
	load_question(); // load_question_data
	
	new_question(); // Show question
}

void win_game_screen() {
	in_game_clear();
	gtk_widget_show(btn_back);
	end_game_result = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(end_game_result), "<b>Các bạn đã chiến thắng</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), end_game_result, 1, 3, 1, 2);
	gtk_widget_show(end_game_result);

	result_screen();	
}

void lose_game_screen() {
	in_game_clear();
	gtk_widget_show(btn_back);
	end_game_result = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(end_game_result), "<b>Các bạn đã thua cuộc</b>");
	gtk_table_attach_defaults(GTK_TABLE(table), end_game_result, 1, 3, 1, 2);
	gtk_widget_show(end_game_result);

	result_screen();
}

void result_screen() {
	result_box = gtk_text_view_new();
	gtk_table_attach_defaults(GTK_TABLE(table), result_box, 1, 3, 2, 5);
	GtkTextBuffer *buffer2 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (result_box));
	
	char temp[LENGTH_MSG], another_client[40];
	sprintf(temp, "%s trả lời đúng câu: ", client_name);
	for(int i = 0; i < q_cur; i++) {
		if (strcmp(answer_client[i], client_name) == 0)
			sprintf(temp + strlen(temp), "%d, ", i+1);
	}
	int j = 0;
	for(int i = 0; i < q_cur; i++) {
		if (strcmp(answer_client[i], client_name) != 0) {
			if (j == 0) {
				sprintf(temp + strlen(temp), "--- %s trả lời đúng câu: ", answer_client[i]);
				j++;
			}
			sprintf(temp + strlen(temp), "%d, ", i+1);
		}
	}

	gtk_text_buffer_set_text (buffer2, temp, -1);
	gtk_widget_show(result_box);
}
void end_game_clear() {
	if (end_game_result != NULL) {
		gtk_widget_hide(end_game_result);
		gtk_widget_hide(result_box);
		end_game_result = NULL;
		result_box = NULL;
	}
}

void game_quit(GtkWidget *widget, gpointer *data)
{
	if (room_id != NULL && in_waiting_friend == TRUE) {
		send_back(); // left room
	}
	// Disconnect server
	char message[LENGTH_MSG];
	sprintf(message, "./exit");
    send(client_sock, message, strlen(message), 0);
    gtk_main_quit();
}