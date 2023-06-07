CC = gcc
CFLAGS = -std=c11

all: server

server: server.c
	gcc server.c -o server	
.PHONY: clean
clean:
	rm -f server

