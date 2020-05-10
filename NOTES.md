
Game Protocol

Packet Max size: 53 bytes
Flag Values : PXX, MXX, GXX, CXX, FXX, 

XX stands for 2digit number that corresponds to a functionality (NOT FUNCTION) id

## Player :

### onMove()
```
Everytime a player changes on of his coordinates a buffer is sent to the server. 
The server broadcasts that buffer acting as an echo
```
Buffer Structure:

``[P01]:[Player ID]:[Player X]:[Player Y]``

###onPlayerUpdateStats()

```
When a player takes Damage from a monster or gets stats upgrades from a chest the client sends a message to the server
The server echoes this message to every client
```

Buffer Structure:

``[P02]:....:`` (Player struct)
###onPlayerDeath()

``When a player dies it sends a Message to the server which is getting broadcasted to all active connections``

In that situation the player needs to still stay in the game and attend its process

Buffer Structere:
``[P03][Player ID][isDead == TRUE]``


## Monster :

onMonsterDeath()
``When a player dies the client sends a Message to the server which is getting broadcasted to all connections``

Buffer Structure:
``[M01][MONS ID][ISDEAD == TRUE]``

onMonsterUpdateStats()
``[M02][MONSID]....[MONSTRUCT]``
## Chest :

onChestOpen()
``[C01][CHID][ISOPEN==TRUE]``
## Game:
```
I might need CFG FOR THAT
```
onGameStart()
onGameOver()
onGameSave()

SERVER
- QUEUE AWAITS FOR INPUT ON SERVER
- Receives message
- Enqueue
- Handle the message and echoes the response to all clients
- Dequeues the buffer

CLIENT NEEDS TO BE MULTITHREADED
- RECEIVER ---> Handles incoming messages
- Main thread ---> continues playing the game and sends messages to the socket
