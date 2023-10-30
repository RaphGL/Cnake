#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCORESIZ 10

typedef struct {
  char player[3];
  size_t score;
} Score;

void scoreboard_load(Score dst[SCORESIZ], char *filepath);
void scoreboard_save(const Score scoreboard[SCORESIZ], char *filepath);
void scoreboard_add_score(Score scoreboard[SCORESIZ], char player[3],
                          size_t score);
void scoreboard_draw(const Score scoreboard[SCORESIZ]);

#endif
