CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm -pthread

all:	ser cli

cli:	tcp_client.c
	${CC} $(LIBS) -o client tcp_client.c

ser:	tcp_server.c
	${CC} $(LIBS) -o server tcp_server.c

clean:
	rm -f *.o *~
