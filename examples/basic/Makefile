# INCLUDE_DIRS=../../include
# CC=gcc
# CXX=g++

# basic: basic.c pixcon
# 	$(CC) -c $< -o build/$@.o -I$(INCLUDE_DIRS)
# 	$(CC) $@.o -o $@ -L./build -lpixcon 

# pixcon: ../../src/pixcon.cpp $(INCLUDE_DIRS)/pixcon.h build
# 	$(CXX) -c -Wall -fpic $< -o build/$@.o -I$(INCLUDE_DIRS)
# 	$(CXX) -shared -o build/lib$@.so build/$@.o

# build:
# 	@mkdir $@

INCLUDE_DIRS = ../../include
CC = gcc
CXX = g++
CFLAGS = -Wall -I$(INCLUDE_DIRS) -ggdb -static

all: basic

basic: build/basic.o build/pixcon.o
	$(CC) $^ -o build/$@ 

build/basic.o: basic.c | build
	$(CC) -c basic.c -o $@ $(CFLAGS)

build/pixcon.o: ../../src/pixcon.c $(INCLUDE_DIRS)/pixcon.h | build
	$(CC) -c $< -o $@ -I$(INCLUDE_DIRS) $(CFLAGS)

build:
	mkdir -p build

clean:
	rm -rf build basic basic.o
