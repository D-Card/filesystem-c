CC=gcc
CFLAGS=-Wall -Wextra -Werror -ansi -pedantic
TARGET=proj2

all:
	$(CC) $(CFLAGS) filesystem.c AVL.c LL.c -o $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)
