CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -O2
CXXDEBUGFLAGS = -fsanitize=address -fsanitize=undefined
LDFLAGS = -lncursesw

.PHONY: all

all: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $? -o cnakepp

debug: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CXXDEBUGFLAGS) $? -o cnakepp_debug

clean:
	$(RM) cnakepp cnakepp_debug
