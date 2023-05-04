CC=gcc
CFLAGS=-Wall -g

default: clean debug

clean:
	rm -rf bin/debug/*

debug:
	$(CC) $(CFLAGS) -o bin/debug/main main.c *.h -lglut -lGL -lGLU
