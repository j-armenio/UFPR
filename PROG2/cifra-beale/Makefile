CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

all: beale.c
	$(CC) $(CFLAGS) beale.c -o beale

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f beale