TARGET = PioTGuard-Server
CC = gcc
CFLAGS = -Wall -pthread

INCLUDES = -Ilib
LIBS = -Llib -lmosquitto

SRCS = src/server.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)