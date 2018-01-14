CC = gcc
CFLAGS = -O3 -Wall

all: clut.o

clut.o: clut.c clut.h
	$(CC) $(CFLAGS) clut.c -c

.phony: clean

clean:
	rm -f clut.o
