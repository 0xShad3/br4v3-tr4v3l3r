# Collect the binaries
CGSRC = $(wildcard src/*.c)
CSSRC = $(wildcard server/*.c)

CC = gcc

game.bin: $(CGSRC)
	$(CC) -g -Wall -o game.bin $(CGSRC)

server.bin: $(CSSRC)
	$(CC) -g  -Wall -o server.bin $(CSSRC)
