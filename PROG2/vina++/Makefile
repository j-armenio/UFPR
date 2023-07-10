# Variables
CC = gcc
CFLAGS = -Wall -g -D_DEFAULT_SOURCE # -std=c99
LIBS = src/libmember.c src/liboptions.c src/libutil.c src/libbinary.c src/libmemUtil.c
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