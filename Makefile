VERSION=0.1
CC=g++
SRC=main.cpp subnet.h
LFLAGS=-Wall
TITLE=subcalc
EXECUTABLE=bin/$(TITLE)

.PHONY: all release debug install fmt

all: release debug

release:
	@echo Build release
	@mkdir -p bin
	@$(CC) $(LFLAGS) -O3 $(SRC) -o $(EXECUTABLE)
	@echo success

debug:
	@echo Build debug
	@mkdir -p bin
	@$(CC) $(LFLAGS) -g $(SRC) -o $(EXECUTABLE)_debug
	@echo success

fmt:
	clang-format -i -style=file *.cpp *.h

install:
	@echo Installing subcalc...
	@make release
	@cp $(EXECUTABLE) /usr/bin/
	@echo success

clean:
	@echo Cleaning up
	rm -rf ./bin /usr/bin/$(TITLE)
	@echo success

uninstall:
	@echo Uninstalling subcalc...
	rm -rf /usr/bin/$(TITLE)
	@echo success
