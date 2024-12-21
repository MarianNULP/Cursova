CC = gcc
CFLAGS = -O2 -Wall

OBJ = src/main.o src/fft.o src/window.o src/io.o

all: project

project: $(OBJ)
	$(CC) $(CFLAGS) -o project $(OBJ) -lm

clean:
	rm -f $(OBJ) project
