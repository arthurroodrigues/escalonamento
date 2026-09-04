CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRC = main.c simulator.c
TARGET = scheduler

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o *.out