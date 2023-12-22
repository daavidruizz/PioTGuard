CC = gcc
CFLAGS = -Wall -pthread

all: demo

demo: src/server.o lib/lcd1602.o
	$(CC) $^ $(LIBS) -o $@ -pthread

src/main.o: src/server.c lib/lcd1602.h
	$(CC) $(CFLAGS) -c $< -o $@

lib/lcd1602.o: lib/lcd1602.c lib/lcd1602.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o lib/*.o demo