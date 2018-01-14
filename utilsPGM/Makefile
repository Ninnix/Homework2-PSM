CC = gcc
CFLAGS = -O3 -Wall

all: pgm.o

pgm.o: pgm.c pgm.h
	$(CC) $(CFLAGS) pgm.c -c

.phony: clean

clean:
	rm -f pgm.o
