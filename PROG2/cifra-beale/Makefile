# Variables
CC = gcc
CFLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE
LIBS = src/libutil.c src/liblist.c src/libcrypto.c
OBJS = src/beale.o

# Compilation Rules
all: beale

beale: $(OBJS)
	$(CC) $(CFLAGS) -o beale $(OBJS) $(LIBS)

beale.o: beale.c
	$(CC) $(CFLAGS) -c beale.c

clean:
	-rm -f src/*~ src/*.o

purge: clean
	-rm -f beale


# Utils Commands
testEnc:
	./beale -e -b test/LivroCifra -m test/MensagemOriginal -o test/MensagemCodificada -c test/ArquivoDeChaves

testDecKey:
	./beale -d -i test/MensagemCodificada -c test/ArquivoDeChaves -o test/MensagemDecodificada

testDecBook:
	./beale -d -i test/MensagemCodificada -b test/LivroCifra -o test/MensagemDecodificada