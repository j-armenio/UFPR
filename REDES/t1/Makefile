CC = gcc
CFLAGS = -Wall -Iinclude -g

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

SERVER_OBJS = $(filter-out $(BUILD_DIR)/client.o, $(OBJS))
CLIENT_OBJS = $(filter-out $(BUILD_DIR)/server.o, $(OBJS))

EXECUTABLES = $(BIN_DIR)/client $(BIN_DIR)/server

all: $(EXECUTABLES)

# Compilação do cliente
$(BIN_DIR)/client: $(CLIENT_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compilação do servidor
$(BIN_DIR)/server: $(SERVER_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Geração dos arquivos .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Diretórios necessários
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/$(EXECUTABLES)
