CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
TARGET = feffi-map

SRC = src/main.c src/iface.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
