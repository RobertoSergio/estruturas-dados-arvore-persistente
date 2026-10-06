CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

TARGET = programa

SRC = \
	src/main.cpp \
	src/core/PersistentBST.cpp \
	src/io/InputParser.cpp

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET) $(INPUT)

clean:
	rm -f $(TARGET)