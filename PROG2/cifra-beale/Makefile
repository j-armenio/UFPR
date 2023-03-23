CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

all: beale.c libGeneral.h
	$(CC) $(CFLAGS) beale.c libGeneral.c -o beale

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f beale