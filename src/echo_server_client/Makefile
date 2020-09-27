BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = .
CC = gcc
CCFLAGS = -Wall -Werror

CLIENT_OBJS = $(BUILD_DIR)/client.o
SERVER_OBJS = $(BUILD_DIR)/server.o

.PHONY: mkdir, clear, all


all: mkdir $(BIN_DIR)/clinet.out $(BIN_DIR)/server.out

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CCFLAGS) $< -c -o $@

$(BIN_DIR)/clinet.out: $(CLIENT_OBJS)
	$(CC) $(CCFLAGS) $< -o $@

$(BIN_DIR)/server.out: $(SERVER_OBJS)
	$(CC) $(CCFLAGS) $< -o $@



mkdir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)