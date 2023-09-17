# Variables
CC = gcc
CFLAGS = -Wall -g -D_DEFAULT_SOURCE
LIBS = src/libaux.c
OBJS = src/main.o

# Compilation Rules
all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	-rm -f src/*~ src/*.o

purge: clean
	-rm -f main