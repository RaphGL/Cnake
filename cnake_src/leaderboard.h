#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// maximum amount of scores displayed in leaderboard
// ie: the number of rows in leaderboard
#define SCORESIZ 10

typedef struct {
  char player[3];
  size_t score;
} Score;

void leaderboard_load(Score dst[static SCORESIZ], char *filepath);
void leaderboard_save(const Score leaderboard[static SCORESIZ], char *filepath);
void leaderboard_add_score(Score leaderboard[static SCORESIZ], char player[3],
                           size_t score);
void leaderboard_draw(const Score leaderboard[static SCORESIZ], int maxy,
                      int maxx);

#endif
