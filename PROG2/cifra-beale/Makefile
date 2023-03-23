CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

all: beale.c libGeneral.c libGeneral.h libTest.c libTest.h libAVL.c libAVL.h
	$(CC) $(CFLAGS) beale.c libGeneral.c libTest.c libAVL.c -o beale

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f beale