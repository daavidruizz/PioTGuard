CC = gcc
CFLAGS = -Wall -O2

all: demo

demo: src/mainlcd.o lib/lcd1602.o
	$(CC) $^ $(LIBS) -o $@

src/main.o: src/mainlcd.c lib/lcd1602.h
	$(CC) $(CFLAGS) -c $< -o $@

lib/lcd1602.o: lib/lcd1602.c lib/lcd1602.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o lib/*.o demo