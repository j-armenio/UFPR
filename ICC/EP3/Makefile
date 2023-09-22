# PROGRAMA
    PROG = interpola

# Compilador
    CC     = gcc
    CFLAGS = -Wall -g -D_DEFAULT_SOURCE -O3 -mavx -march=native
    LFLAGS = -lm

# Lista de arquivos para distribuição
DISTFILES = *.c *.h LEIAME* Makefile
DISTDIR = `basename ${PWD}`

TARGET = src/main.o
LIBS = 

.PHONY: all main clean purge dist

# Regras de Compilação
all: main

main: $(TARGET)
	@echo "Compilando programa ..."
	$(CC) $(CFLAGS) -o main $(TARGET) $(LIBS) $(LFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(LFLAGS)

clean:
	@echo "Limpando sujeira ..."
	@rm -f *~ *.bak

purge:  clean
	@echo "Limpando tudo ..."
	@rm -f $(PROG) *.o core a.out $(DISTDIR) $(DISTDIR).tar

dist: purge
	@echo "Gerando arquivo de distribuição ($(DISTDIR).tar) ..."
	@ln -s . $(DISTDIR)
	@tar -cvf $(DISTDIR).tar $(addprefix ./$(DISTDIR)/, $(DISTFILES))
	@rm -f $(DISTDIR)
