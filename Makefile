CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm -pthread

all:	ser cli

cli:	client.c
	${CC} $(LIBS) -o client client.c

ser:	server.c
	${CC} $(LIBS) -o server server.c

clean:
	rm -f *.o *~
