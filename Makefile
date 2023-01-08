CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -O2
LDFLAGS = -lncursesw

.PHONY: all

all: src/main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $?
clean:
	rm -f a.out
