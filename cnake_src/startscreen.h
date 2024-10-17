#ifndef STARTSCREEN_H
#define STARTSCREEN_H

typedef enum {
  SP_ONE_PLAYER,
  SP_TWO_PLAYERS,
  SP_LEADERBOARD,
  SP_NONE
} SelectedPage;

SelectedPage draw_startscreen(int key, int maxy, int maxx);

#endif
