CC = gcc
CFLAGS = -std=c18 -Werror 


all: server

server: server.c
	gcc server.c -o server	
	
.PHONY: clean
clean:
	rm -f server

