CC = clang
BUILD_DIR = build
SRC = wordHunt.c
TARGET = wordHunt

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET)
