CC = gcc
CFLAGS =  -Wall -g -std=c90

verificaexpressao: main.o libpilha.o
	$(CC)  main.o libpilha.o -o verificaexpressao 

main.o : main.c	
	$(CC) $(CFLAGS) -c main.c

libpilha.o : libpilha.c
	$(CC) $(CFLAGS) -c libpilha.c

clean :
	rm -f *.o verificaexpressao


