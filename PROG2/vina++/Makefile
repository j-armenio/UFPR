# Variables
CC = gcc
CFLAGS = -Wall -g -D_DEFAULT_SOURCE # -std=c90
LIBS = src/liboptions.c src/libdirectory.c src/libbackup.c src/libtest.c
OBJS = src/vina++.o

TESTOBJ = src/testExec.o

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

test: $(TESTOBJ)
	$(CC) $(CFLAGS) -o testExec $(TESTOBJ) $(LIBS)