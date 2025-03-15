CFLAGS = -Wall -g

# Lista de arquivos objeto
objs = testafila.o queue.o

# Nome do executável
TARGET = testafila

# Regra padrão
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(objs)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para gerar testafila.o
testafila.o: testafila.c queue.h
	$(CC) $(CFLAGS) -c $<

# Regra para gerar queue.o
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $<

# Regra para limpar arquivos gerados
clean:
	rm -f $(objs) *~

# Regra para limpar tudo, incluindo o executável
purge: clean
	rm -f $(TARGET)