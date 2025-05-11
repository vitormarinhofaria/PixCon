INCLUDE_DIRS = ../../include
CC = gcc
CXX = g++
CFLAGS = -Wall -I$(INCLUDE_DIRS) -ggdb -static

all: image

image: build/image.o build/pixcon.o
	$(CC) $^ -o build/$@ -lm

build/image.o: image.c | build
	$(CC) -c image.c -o $@ $(CFLAGS)

build/pixcon.o: ../../src/pixcon.c $(INCLUDE_DIRS)/pixcon.h | build
	$(CC) -c $< -o $@ -I$(INCLUDE_DIRS) $(CFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build
