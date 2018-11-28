CC = gcc
LIBS = -lgthread-2.0 `pkg-config gtk+-2.0 --cflags --libs`

all:	client	server	tcp-client

client:	client.c

	${CC} -lm -pthread client.c $(LIBS) -o client  
server:	server.c
	${CC} -lm -pthread server.c  -o server  

tcp-client: tcp-client.c
	gcc -w -g -o tcp-client -lgthread-2.0 `pkg-config gtk+-3.0 --cflags --libs` tcp-client.c

clean:
	rm -f *.o *~ client server