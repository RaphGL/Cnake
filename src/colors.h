#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>

#define GREEN_FG COLOR_PAIR(1)
#define GREEN_BG COLOR_PAIR(2)
#define YELLOW_FG COLOR_PAIR(3)

// initializes default cnake colors if the terminal supports it
void cnake_init_colors(void);

#endif
