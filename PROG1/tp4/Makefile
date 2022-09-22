CC = gcc
CFLAGS =  -Wall -g -std=c90

testa_fila: testa_fila.o libfila.o
	$(CC)  testa_fila.o libfila.o -o testa_fila 

testa_fila.o : testa_fila.c	
	$(CC) $(CFLAGS) -c testa_fila.c

libfila.o : libfila.c
	$(CC) $(CFLAGS) -c libfila.c

clean :
	rm -f *.o testa_fila
