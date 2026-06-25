CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = memory-leak-hunter
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC) levels.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) scores.json

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run