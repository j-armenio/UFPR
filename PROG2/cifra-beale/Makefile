CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

all: beale.c libgeneral.c libgeneral.h libtest.c libtest.h liblist.c liblist.h
	$(CC) $(CFLAGS) beale.c libgeneral.c libtest.c liblist.c -o beale

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f beale