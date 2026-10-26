# Makefile – LibraryManagement (C++11)
CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude
SRC_DIR  := src
BIN_DIR  := bin
TEST_DIR := tests

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))
TARGET   := $(BIN_DIR)/library

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

# Mỗi file tests/test_*.cpp được biên dịch riêng cùng với các .o (trừ main.o)
TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TEST_SRCS))
LIB_OBJS  := $(filter-out $(BIN_DIR)/main.o,$(OBJS))

$(BIN_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(LIB_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do echo "== $$t"; ./$$t || exit 1; done

clean:
	rm -rf $(BIN_DIR)

.PHONY: all run test clean
