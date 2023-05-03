# Variables
CC = gcc
CFLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE
LIBS = src/liboptions.c
OBJS = src/vina++.o

# Compilation Rules
all: vina++

vina++: $(OBJS)
	$(CC) $(CFLAGS) -o vina++ $(OBJS) $(LIBS)

vina++.o: vina++.c
	$(CC) $(CFLAGS) -c vina++.c

clean:
	-rm -f src/*~ src/*.o

purge: clean
	-rm -f vina++