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

void convert_client_detail(char *data) {
	int i = 0, j, k = 0;
	char element[10];
	while(data[i]) {
		memset(element, 0, strlen(element));
		j = 0;
		while(data[i] != '#') {
			element[j++] = data[i++]; 
		}
		i++;
		strcpy(client_arr[k++].name, element);   
	}
	client_num = k;
}