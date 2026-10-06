CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

TARGET = programa

SRC = \
	src/main.cpp \
	src/core/PersistentBST.cpp \
	src/core/PersistenceManager.cpp \
	src/io/InputParser.cpp

OFFICIAL_TESTS = 01 02 03 04 05

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET) $(INPUT)

test: build
	@for i in $(OFFICIAL_TESTS); do \
		echo "Teste $$i:"; \
		./$(TARGET) tests/oficiais/teste$$i.in > /tmp/saida$$i.out; \
		diff -u tests/oficiais/teste$$i.out /tmp/saida$$i.out || exit 1; \
	done
	@echo "Todos os testes oficiais passaram."

clean:
	rm -f $(TARGET)

.PHONY: build run test clean