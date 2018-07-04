VERSION=0.1
CC=g++
SRC=main.cpp subnet.h
LFLAGS=-Wall
EXECUTABLE=subcalc

.PHONY: all release debug

all:
	make release
	make debug

release:
	$(CC) $(LFLAGS) -O3 $(SRC) -o $(EXECUTABLE)_amd64

debug:
	$(CC) $(LFLAGS) -g $(SRC) -o $(EXECUTABLE)_debug

clean:
	rm -f $(EXECUTABLE)_amd64 $(EXECUTABLE)_debug
