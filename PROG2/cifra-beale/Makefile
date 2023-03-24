CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

all: beale.c libgeneral.c libgeneral.h libtest.c libtest.h libavl.c libavl.h
	$(CC) $(CFLAGS) beale.c libgeneral.c libtest.c libavl.c -o beale

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f beale