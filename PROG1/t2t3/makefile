# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = mundo.o libconjunto.o  liblistaord.o libfila.o libaleat.o
     
main: mundo.o libconjunto.o  liblef.o libfila.o libaleat.o
	$(CC) -o mundo mundo.o libconjunto.o  liblef.o libfila.o libaleat.o $(LDFLAGS)

teste_fila: testa_fila.o libfila.o
	$(CC) -o testa_fila testa_fila.o libfila.o $(LDFLAGS)

teste_conjunto: testa_conjunto.o libconjunto.o libaleat.o
	$(CC) -o testa_conjunto testa_conjunto.o libconjunto.o libaleat.o $(LDFLAGS)

teste_lef: testa_lef.o liblef.o libaleat.o
	$(CC) -o testa_lef testa_lef.o liblef.o libaleat.o $(LDFLAGS)

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

libaleat.o: libaleat.c
	$(CC) -c $(CFLAGS) libaleat.c

mundo.o: mundo.c
	$(CC) -c $(CFLAGS) mundo.c

clean:
	rm -f $(objects) mundo
	