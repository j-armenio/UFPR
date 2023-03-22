CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

all: main.c
	$(CC) $(CFLAGS) main.c -o main

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f main