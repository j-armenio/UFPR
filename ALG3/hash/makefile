CFLAGS = -Wall -g -std=c99
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = main.o hasht.o
     
all: $(objects)
	$(CC) -o myht $(objects) $(LDFLAGS)

hasht.o: hasht.c
	$(CC) -c $(CFLAGS) hasht.c

clean:
	rm -f $(objects) index