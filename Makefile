CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I vendor/raylib -I src
LDFLAGS   := -L vendor/raylib -lraylib -lm -lpthread -ldl -lrt -lX11
SRC       := src/main.cpp \
            src/core/resource.cpp \
            src/core/layout.cpp \
            src/core/screen.cpp
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
TARGET    := build/cardgame

# Test configuration
TEST_CXXFLAGS := -std=c++17 -Wall -Wextra -g -O0 -I src
TEST_SRC      := src/test_main.cpp
TEST_LOGIC_SRC := $(wildcard src/logic/test_*.cpp)
TEST_TARGET   := build/run_tests

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

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(TEST_LOGIC_SRC) | build
	$(CXX) $(TEST_CXXFLAGS) -o $@ $^ -lm

clean:
	rm -rf build

format:
	clang-format -i src/**/*.cpp src/**/*.h

format-check:
	clang-format --dry-run --Werror src/**/*.cpp src/**/*.h

.PHONY: all run debug test clean format format-check
