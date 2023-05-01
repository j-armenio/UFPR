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


# Test Commands
testEnc:
	./beale -e -b test/LivroCifra.txt -m test/MensagemOriginal -o test/MensagemCodificada -c test/ArquivoDeChaves

testDecKey:
	./beale -d -i test/MensagemCodificada -c test/ArquivoDeChaves -o test/MensagemDecodificada

testDecBook:
	./beale -d -i test/MensagemCodificada -b test/LivroCifra.txt -o test/MensagemDecodificada

valgrindEnc:
	valgrind --leak-check=full --show-leak-kinds=all ./beale -e -b test/LivroCifra.txt -m test/MensagemOriginal -o test/MensagemCodificada -c test/ArquivoDeChaves

valgrindDecKey:
	valgrind --leak-check=full ./beale -d -i test/MensagemCodificada -c test/ArquivoDeChaves -o test/MensagemDecodificada

valgrindDecBook:
	valgrind --leak-check=full ./beale -d -i test/MensagemCodificada -b test/LivroCifra.txt -o test/MensagemDecodificada