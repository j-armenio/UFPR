CFLAGS = -Wall -g -std=c99
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = main.o myht.o
     
all: $(objects)
	$(CC) -o main $(objects) $(LDFLAGS)

myht.o: myht.c
	$(CC) -c $(CFLAGS) myht.c

clean:
	rm -f $(objects) index