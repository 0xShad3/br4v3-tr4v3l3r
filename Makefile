# Collect the binaries
CGSRC = $(wildcard src/*.c)
CSSRC = $(wildcard server/*.c)

CC = gcc

game.bin: $(CGSRC)
	$(CC) $(CGSRC) -gstabs -Wall -lpthread -lcrypto -lssl -no-pie -o game.bin 

server.bin: $(CSSRC)
	$(CC) $(CSSRC)  src/events_handler.c src/mode_handle.c src/net_protocol.c src/custom_effects.c src/player.c src/client.c src/util.c src/login.c src/chest.c src/map.c src/game_logic.c src/monster.c  -gstabs -lpthread -lcrypto -lssl -no-pie -o server.bin 
