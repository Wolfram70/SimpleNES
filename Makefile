CC = g++
CFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lasound
DEBUG = -g
WARN = -Wall
SRC = src/*.cpp
TARGET = emu

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o bin/$(TARGET)

# release build
release: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o bin/$(TARGET)

# debug build
debug: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(DEBUG) $(WARN) -o bin/$(TARGET)

# clean the build
clean:
	rm -f bin/$(TARGET)