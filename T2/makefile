CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = main.c \
      lib/fornecidas.c \
      $(wildcard lib/funcs/*.c) \
      $(wildcard lib/tools/*.c)

TARGET = programa

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)