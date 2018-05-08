PATH_INCLUDES = Includes
PATH_SOURCES = Sources

BINARY = chip8-emulator
INCLUDES = -I$(PATH_INCLUDES)
LIBRARIES = -lSDL2
SOURCES = $(shell find $(PATH_SOURCES) -mindepth 1)

CC = gcc
CCFLAGS += -W -Wall

debug: CCFLAGS += -g -O0
debug: all

release: CCFLAGS += -O2 -DNDEBUG
release: all

all:
	$(CC) $(CCFLAGS) $(INCLUDES) $(SOURCES) $(LIBRARIES) -o $(BINARY)

clean:
	rm -f $(BINARY)
