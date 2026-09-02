CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ilib
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/vector_char.c $(SRC_DIR)/vector_string.c $(SRC_DIR)/vector_token.c $(SRC_DIR)/list_string.c $(SRC_DIR)/cd.c $(SRC_DIR)/tokenizer.c $(SRC_DIR)/executor.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = program

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all

.PHONY: all clean reduild
