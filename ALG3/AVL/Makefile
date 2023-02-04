CC = gcc
CGLAGS = -Wall -g -std=c90

myavl: myavl.c libAvlTree.c libAvlTree.h
	$(CC) $(CFLAGS) myavl.c libAvlTree.c -o myavl

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f myavl