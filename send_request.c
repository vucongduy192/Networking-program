




void send_name(GtkWidget *widget, gpointer *data) {
	// Get username
	const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(widget));
	char new_client_cmd[LENGTH_MSG];
	sprintf(new_client_cmd, "./new_client %s", send_buffer);
	int n = send(client_sock, new_client_cmd, strlen(new_client_cmd),0);
	//puts(new_client_cmd);
}

void send_room(GtkWidget *widget, gpointer *data) {
	//printf("\r%d", *((int *)data));
	char join_room_cmd[LENGTH_MSG];
	sprintf(join_room_cmd, "./join_room %d", *((int *)data));
	send(client_sock, join_room_cmd, strlen(join_room_cmd),0);
	puts(join_room_cmd);
}

void send_msg() {
    char message[LENGTH_MSG] = {};
    const gchar *send_buffer = gtk_entry_get_text(GTK_ENTRY(entry_msg));
    append_message(send_buffer);
	sprintf(message, "./new_message %s", send_buffer);
    send(client_sock, message, strlen(message), 0);
	puts(message);
	gtk_entry_set_text(GTK_ENTRY(entry_msg), "");
	
}




