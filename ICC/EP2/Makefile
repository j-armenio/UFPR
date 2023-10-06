# PROGRAMA
    PROG = src/perfEG
	PROGLOC = perfEG

# Compilador
    CC     = gcc
    CFLAGS = -Wall -g -D_DEFAULT_SOURCE -O3 -mavx -march=native
    LFLAGS = -lm

# Lista de arquivos para distribuição
DISTFILES = *.c *.h LEIAME* Makefile
DISTDIR = `basename ${PWD}`

.PHONY: all clean purge dist 

all: $(PROG)

$(PROG): $(PROG).o
	@echo "Gerando o executável $@ ..."
	@$(CC) $(CFLAGS) -o ${PROGLOC} $^ $(LFLAGS)

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
