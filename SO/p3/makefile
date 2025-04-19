# Compilador
CC = gcc
# Flags de compilação
CFLAGS = -Wall -g
# Nome do executável
TARGET = pingpong-dispatcher

# Arquivos objeto
OBJS = pingpong-dispatcher.o ppos_core.o queue.o

# Regra padrão
all: $(TARGET)

# Ligar todos os objetos para criar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regras de compilação para cada arquivo fonte
pingpong-dispatcher.o: pingpong-dispatcher.c ppos.h
	$(CC) $(CFLAGS) -c $<

ppos_core.o: ppos_core.c ppos_data.h ppos.h queue.h
	$(CC) $(CFLAGS) -c $<

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $<

# Limpar arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run