#include "startscreen.h"
#include "colors.h"
#include <ncurses.h>
#include <string.h>

static char selections[3][20] = {
    "    1 Player       ",
    "    2 Players      ",
    "    Leaderboard    ",
};

static const int no_selections = sizeof(selections) / sizeof(selections[0]);
static int selected = 0;

static SelectedPage draw_selectmenu(int key, int y, int x) {
  switch (key) {
  case 'j':
  case 's':
  case KEY_DOWN:
    if (selected < no_selections - 1) {
      ++selected;
    }
    break;

  case 'k':
  case 'w':
  case KEY_UP:
    if (selected > 0) {
      --selected;
    }
    break;

  case '\n':
    switch (selected) {
    case 0:
      return ONE_PLAYER;
      break;

    case 1:
      return TWO_PLAYERS;
      break;

    case 2:
      return LEADERBOARD;
      break;
    }
    break;
  }

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

  return NONE;
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
  const char *instructions = "↑ w \t ↓ s \t Enter";
  mvprintw(y, x - strlen(instructions) / 2, "%s", instructions);
}

SelectedPage draw_startscreen(int key, int maxy, int maxx) {
  draw_snake(maxy / 4, maxx / 2);
  draw_instructions(maxy - 1, maxx / 2);
  return draw_selectmenu(key, maxy / 2, maxx / 2);
}
