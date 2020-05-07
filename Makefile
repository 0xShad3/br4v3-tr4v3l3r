# Collect the binaries
CGSRC = $(wildcard src/*.c)
CSSRC = $(wildcard server/*.c)

CC = gcc


game.bin: $(CGSRC)
	$(CC) -g -Wall -o bin/game.bin $(CGSRC)

server.bin: $(CSSRC)
	$(CC) -g  -Wall -o bin/server.bin $(CSSRC)
