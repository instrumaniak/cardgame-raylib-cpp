RAYLIB_PATH ?= ./vendor/raylib

RAYLIB_INCLUDE_PATH = ${RAYLIB_PATH}
RAYLIB_LIB_PATH = ${RAYLIB_PATH}

CC = g++
CFLAGS = -Isrc -I${RAYLIB_INCLUDE_PATH} -Wall -std=c++17
LDFLAGS = -L${RAYLIB_LIB_PATH}
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

build/game: src/game.cpp | build
	${CC} ${CFLAGS} -o $@ $< $(LDFLAGS) $(LIBS)

build:
	mkdir -p build

run: build/game
	./build/game

clean:
	rm -rf build

