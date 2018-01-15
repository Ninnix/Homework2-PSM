CC = gcc
CFLAGS = -O3 -l OpenCL
CLUT = ../clut/
PGM = ../utilsPGM/
OBJ = $(CLUT)clut.o
OBJ2 = $(PGM)pgm.o

blurring: blurring.c
	$(CC) $(CFLAGS) -I$(CLUT) -I$(PGM) blurring.c $(OBJ) $(OBJ2) -o blurring 

.phony: clean

clean:
	rm -f blurring 
