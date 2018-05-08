PATH_INCLUDES = Includes
PATH_SOURCES = Sources

CC = gcc
CCFLAGS += -W -Wall -I$(PATH_INCLUDES)

BINARY = chip8-emulator
SOURCES = $(shell find $(PATH_SOURCES) -mindepth 1)

debug: CCFLAGS += -g -O0
debug: all

release: CCFLAGS += -O2 -DNDEBUG
release: all

all:
	$(CC) $(CCFLAGS) $(SOURCES) -o $(BINARY)

clean:
	rm -f $(BINARY)
