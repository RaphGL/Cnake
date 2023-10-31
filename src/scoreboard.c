#include "scoreboard.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scoreboard_load(Score dst[SCORESIZ], char *filepath) {
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

void scoreboard_save(const Score scoreboard[SCORESIZ], char *filepath) {
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

void scoreboard_add_score(Score scoreboard[SCORESIZ], char player[3],
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
Score scoreboard_register_score(void);

// TODO
void scoreboard_draw(const Score scoreboard[SCORESIZ]);
