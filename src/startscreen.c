#include "colors.h"
#include <ncurses.h>
#include <string.h>

typedef enum { ONE_PLAYER, ONE_PLAYERS, LEADERBOARD } SelectedPage;

static char selections[][20] = {
    "    1 Player       ",
    "    2 Players      ",
    "    Leaderboard    ",
};

static const int no_selections = sizeof(selections) / sizeof(selections[0]);
static int selected = 0;

static void draw_selectmenu(int y, int x) {
  for (int i = 0; i < no_selections; i++) {
    // highlight selected option on menu
    if (i == selected) {
      attron(GREEN_BG | A_BOLD);
    }

    mvprintw(y++ - no_selections / 2, x - sizeof(selections[i]) / 2, "%s",
             selections[i]);

    if (i == selected) {
      attroff(GREEN_BG | A_BOLD);
    }
  }
}

// X and Y are the center points for the snake ascii art
static void draw_snake(int y, int x) {
  char *snake[] = {
      "      ____     ",
      "_,.-'`_ o `;__,",
      " _.-'` '---'  '",
      "     Cnake     ",
  };

  attron(GREEN_FG);
  for (int i = 0; i < 4; i++) {
    mvprintw(y - 2, x - 15 / 2, "%s", snake[i]);
    y++;
  }
  attroff(GREEN_FG);
}

static void draw_instructions(int y, int x) {
  const char *instructions = "↑ w \t ← a \t ↓ s \t → d";
  mvprintw(y, x - strlen(instructions) / 2, "%s", instructions);
}

void draw_startscreen(int maxy, int maxx) {
  draw_snake(maxy / 4, maxx / 2);
  draw_instructions(maxy - 1, maxx / 2);
  draw_selectmenu(maxy / 2, maxx / 2);
}
