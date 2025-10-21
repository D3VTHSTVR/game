# Makefile for Cross-Language Game System

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET = game
SRC_DIR = src/cpp
BUILD_DIR = build
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# default target
all: $(TARGET)

# create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f data/*.txt

# test prolog rules
test-prolog:
	swipl -s src/prolog/rules.pl -g "run_tests,halt"

# test scheme AI
test-scheme:
	scheme --load src/scheme/ai.scm

# run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean test-prolog test-scheme run

