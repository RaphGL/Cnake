#include "leaderboard.h"
#include "colors.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void leaderboard_load(Score dst[static SCORESIZ], char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (!f) {
    memset(dst, 0, sizeof(Score) * SCORESIZ);
    return;
  }

  if (fread(dst, sizeof(Score), SCORESIZ, f) != SCORESIZ) {
    memset(dst, 0, sizeof(Score) * SCORESIZ);
    fclose(f);
    return;
  }

  fclose(f);
}

void leaderboard_save(const Score scoreboard[static SCORESIZ], char *filepath) {
  FILE *f = fopen(filepath, "w");
  if (!f) {
    return;
  }

  if (fwrite(scoreboard, sizeof(Score), SCORESIZ, f) != SCORESIZ) {
    fclose(f);
    return;
  }

  fclose(f);
}

void leaderboard_add_score(Score scoreboard[static SCORESIZ], char player[3],
                           size_t score) {
  for (size_t i = 0; i < SCORESIZ; i++) {
    if (scoreboard[i].score < score) {
      // shifts array left to acommodate new score
      memmove(scoreboard + i + 1, scoreboard + i,
              sizeof(Score) * (SCORESIZ - i - 1));
      scoreboard[i].score = score;
      strcpy(scoreboard[i].player, player);
      break;
    }
  }
}

// TODO: make player register score when game ends and return the values
// entered by player
Score leaderboard_register_score(void);

void leaderboard_draw(const Score leaderboard[static SCORESIZ], int maxy,
                      int maxx) {
  char row[SCORESIZ][30] = {0};
  size_t rowlen = 0;
  // generate row text and store into row array
  {
    for (size_t i = 0; i < SCORESIZ; i++) {
      Score s = leaderboard[i];

      // fixes alignment for table
      {
        if (strlen(s.player) == 0) {
          memset(s.player, 'A', sizeof(s.player));
        }

        if (i + 1 < 10) {
          sprintf(row[i], " %ld  │ %s │ %ld ", i + 1, s.player, s.score);
        } else {
          sprintf(row[i], " %ld │ %s │ %ld ", i + 1, s.player, s.score);
        }
      }

      const size_t msglen = strlen(row[i]);
      if (msglen > rowlen) {
        rowlen = msglen;
      }
    }
  }

  // draw table frame
  {
    const int midy = maxy / 2 - SCORESIZ / 2 + 4;
    attron(GREEN_FG);
    mvprintw(midy - 3, maxx / 2 - sizeof("Leaderboard") / 2, "Leaderboard");
    attroff(GREEN_FG);
    for (size_t i = 0; i < SCORESIZ; i++) {
      mvprintw(midy + i, maxx / 2 - rowlen / 2, "%s\n", row[i]);
    }

    const size_t tableminy = midy - 1, tableminx = maxx / 2 - rowlen / 2 - 2;
    const size_t tablemaxy = tableminy + SCORESIZ + 2,
              tablemaxx = tableminx + rowlen;
    for (size_t y = tableminy; y < tablemaxy; y++) {
      for (size_t x = tableminx; x < tablemaxx; x++) {
        if (x == tableminx) {
          if (y == tableminy) {
            mvprintw(y, x, "┌");
          } else if (y == tablemaxy - 1) {
            mvprintw(y, x, "└");
          } else {
            mvprintw(y, x, "│");
          }
        } else if (x == tablemaxx - 1) {
          if (y == tableminy) {
            mvprintw(y, x, "┐");
          } else if (y == tablemaxy - 1) {
            mvprintw(y, x, "┘");
          } else {
            mvprintw(y, x, "│");
          }
        } else if (y == tableminy || y == tablemaxy - 1) {
          mvprintw(y, x, "─");
        }
      }
    }
  }
}
