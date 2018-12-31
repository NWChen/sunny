CC = gcc
CFLAGS = -g -Wall

TARGET = sunny

sunny: sunny.o
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

sunny.o: sunny.c
	$(CC) $(CFLAGS) -c sunny.c

clean:
	$(RM) $(TARGET) *.o
