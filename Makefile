# Collect the binaries
CSRC = $(wildcard src/*.c)
CC = gcc

game.bin: $(CSRC)
	$(CC) -o game.bin $(CSRC)