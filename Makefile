CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++17

SRC = src
INCLUDE = include
BUILD = build

TARGET = $(BUILD)/cpcc

SRCS = $(SRC)/driver.cpp

OBJS = $(BUILD)/driver.o

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/driver.o: $(SRC)/driver.cpp
	@mkdir -p $(BUILD) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
	rm -rf $(BUILD) 

.PHONY: all clean 
