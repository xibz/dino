CC=g++
CFLAGS=-Ofast
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all: dino
dino: emu.o main.o
	$(CC) $(CFLAGS) emu.o main.o -o dino $(LIBS)
emu.o: src/emu.cpp
	$(CC) $(CFLAGS) -c src/emu.cpp $(LIBS)
main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp $(LIBS) 
clean:
	rm -rf *o dino
