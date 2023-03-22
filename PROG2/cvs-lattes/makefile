CC = gcc
CGLAGS = -Wall -g -std=c90 -D_DEFAULT_SOURCE

lattes: lattes.c libResearcher.c libResearcher.h libStrDistance.h
	$(CC) $(CFLAGS) lattes.c libResearcher.c libStrDistance.c -o lattes

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f lattes