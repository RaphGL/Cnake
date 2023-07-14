#ifndef STARTSCREEN_H
#define STARTSCREEN_H

typedef enum { ONE_PLAYER, TWO_PLAYERS, LEADERBOARD, NONE } SelectedPage;

SelectedPage draw_startscreen(int key, int maxy, int maxx);

#endif
