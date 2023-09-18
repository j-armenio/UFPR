# Variables
CC = gcc
CFLAGS = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror
LDFLAGS = -lm
LIBS = src/libaux.c
TARGET = src/main.o

# Compilation Rules
all: main

main: $(TARGET)
	$(CC) $(CFLAGS) $(LDFLAGS) -o main $(TARGET) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c main.c

clean:
	-rm -f src/*~ src/*.o

purge: clean
	-rm -f main