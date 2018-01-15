CC = gcc
CFLAGS = -O3 -l OpenCL
CLUT = ../clut/
OBJ = $(CLUT)clut.o

blurring: blurring.c
	$(CC) $(CFLAGS) -I$(CLUT) blurring.c $(OBJ) -o blurring 

.phony: clean

clean:
	rm -f blurring 
