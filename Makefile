CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -pedantic-errors
CDEBUGFLAGS = -fsanitize=address -fsanitize=undefined -g
LDFLAGS = -lncursesw -lm

.PHONY: all

all: $(wildcard src/*.c)
	$(CC) $(CFLAGS) $(LDFLAGS) -O2 $? -o cnake

debug: $(wildcard src/*.c)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CDEBUGFLAGS) $? -o cnake_debug

clean:
	$(RM) cnake cnake_debug
