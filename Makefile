CC = gcc
CFLAGS = -mconsole -Iinclude -Wall -Wextra

TARGET = kite

SRC = main.c \
sys/kite_sys.c \
commands/*.c \
core/*.c \
diff/*.c \
merge/*.c \
network/*.c \
hash/*.c \
hooks/*.c \
utils/*.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)