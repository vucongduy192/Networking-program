CC = gcc-5
LIBS = `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm
THREAD =  -lm -pthread

all:	client	server	sdl

client:	client.c
	${CC} $(THREAD) -o client client.c	$(LIBS) 
	
server:	server.c
	${CC} $(THREAD) -o server server.c

sdl:	sdl.c
	${CC} $(THREAD) -o sdl sdl.c	$(LIBS) 

clean:
	rm -f *.o *~