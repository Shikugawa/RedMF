CC = g++
CFLAGS = -std=c++17

build: main.cpp
	$(CC) $(CFLAGS) -o rmf main.cpp
