#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * string_multiline(char *str) {
	char *temp = malloc(100*sizeof(char));
	int  i = 0, j = 0;
  puts(str);
	while(i < strlen(str)) {
		temp[j++] = str[i++];
		if (i%5 == 0) {
			temp[j++] = 'b';
		}
	}
  temp[j] = '\0';
	return temp;
}

void main() {
  printf("%s \n", string_multiline("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
}