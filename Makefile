CXX = gcc
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
CXXDEBUGFLAGS = -fsanitize=address -fsanitize=undefined
LDFLAGS = -lncursesw -lm

.PHONY: all

all: $(wildcard src/*.c)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $? -o cnake

debug: $(wildcard src/*.c)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CXXDEBUGFLAGS) $? -o cnake_debug

clean:
	$(RM) cnake cnake_debug
