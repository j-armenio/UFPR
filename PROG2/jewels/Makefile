CC = gcc
ALLEGRO_LIBS = allegro-5 \
	allegro_main-5 \
	allegro_image-5 \
	allegro_font-5 \
	allegro_audio-5 \
	allegro_acodec-5 \
	allegro_ttf-5 \
	allegro_dialog-5
PKG_CONFIG = /usr/bin/pkg-config
CFLAGS = -g -Wall -std=c99 `$(PKG_CONFIG) --cflags $(ALLEGRO_LIBS)`  
LIBS = `$(PKG_CONFIG) --libs $(ALLEGRO_LIBS)`

# arquivos-objeto
OBJ = main.o init.o structs.o destroy.o draw.o run.o update.o menu.o

all:main

main: $(OBJ)
		$(CC) -o main $(OBJ) $(LIBS)

main.o: main.o init.o structs.o destroy.o draw.o run.o update.o
		$(CC) -c main.c $(CFLAGS) 

structs.o: structs.c structs.h
		$(CC) -c structs.c $(CFLAGS)

init.o: init.c init.h
		$(CC) -c init.c $(CFLAGS)

destroy.o: destroy.c destroy.h
		$(CC) -c destroy.c $(CFLAGS)

draw.o: draw.c draw.h
		$(CC) -c draw.c $(CFLAGS)

run.o: run.c run.h
		$(CC) -c run.c $(CFLAGS)

update.o: update.c update.h
		$(CC) -c update.c $(CFLAGS)

menu.o: menu.c menu.h
		$(CC) -c menu.c $(CFLAGS)

clean:
		-rm -f *~ $(OBJ) 

purge: clean
		-rm -f main