CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET   = build/correctness_tests.exe
SOURCES  = main.cpp sources/CorrectnessTests.cpp
HEADERS  = $(wildcard headers/*.hpp headers/*/*.hpp sources/*/*.tpp)

.PHONY: all test asan
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) | build
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run_correctness_test: $(TARGET)
	./$(TARGET)

# Requires a compiler installation with the AddressSanitizer runtime
asan: | build
	$(CXX) -std=c++17 -Wall -Wextra -Wpedantic -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer $(SOURCES) -o build/correctness_tests_asan.exe
	./build/correctness_tests_asan.exe

build:
ifeq ($(OS),Windows_NT)
	if not exist build mkdir build
else
	mkdir -p build
endif
