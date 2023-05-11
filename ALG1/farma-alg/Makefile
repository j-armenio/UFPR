# Variables
CC = gcc
CFLAGS = -Wall -g -D_DEFAULT_SOURCE -std=c90

# Compilation Rules
all:

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	find . -name "*.o" -type f -delete
	find . -type f -executable -delete