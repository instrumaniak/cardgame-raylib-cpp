CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I vendor/raylib -I src
LDFLAGS   := -L vendor/raylib -lraylib -lm -lpthread -ldl -lrt -lX11
SRC       := src/main.cpp \
            src/core/resource.cpp
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
TARGET    := build/cardgame

all: $(TARGET)

build:
	mkdir -p build

$(TARGET): $(OBJ) | build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.cpp | build
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: $(TARGET)

clean:
	rm -rf build

format:
	clang-format -i src/**/*.cpp src/**/*.h

format-check:
	clang-format --dry-run --Werror src/**/*.cpp src/**/*.h

.PHONY: all run debug clean format format-check
