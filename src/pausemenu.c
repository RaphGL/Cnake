#include "pausemenu.h"
#include "colors.h"
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static char options[4][26] = {
    "       Resume          ",
    "       Restart         ",
    "       Main Menu       ",
    "       Quit            ",
};

static const int no_chosen = sizeof(options) / sizeof(options[0]);
static int chosen = 0;
static bool paused = false;

SelectedOption pausemenu_draw(int key, int y, int x) {
  if (key == 'p') {
    paused = !paused;
  }

  if (!paused) {
    return SO_NONE;
  }

  for (;;) {
    switch (key) {
    case 'j':
    case 's':
    case KEY_DOWN:
      if (chosen < no_chosen - 1) {
        ++chosen;
      }
      break;

    case 'k':
    case 'w':
    case KEY_UP:
      if (chosen > 0) {
        --chosen;
      }
      break;

    case '\n':
      paused = false;
      switch (chosen) {
        // Resume
      case 0:
        return SO_RESUME;
        break;

        // Restart
      case 1:
        return SO_RESTART;
        break;

        // MainMenu
      case 2:
        return SO_MAINMENU;
        break;

        // Quit
      case 3:
        raise(SIGTERM);
        break;
      }
      break;
    }

    attron(A_BOLD);
    mvprintw(y - 2 - no_chosen / 2, x - sizeof("PAUSED") / 2, "PAUSED");
    attroff(A_BOLD);
    for (int i = 0; i < no_chosen; i++) {
      // highlight selected option on menu
      if (i == chosen) {
        attron(A_BOLD | A_REVERSE);
      }

      mvprintw(y + i - no_chosen / 2, x - strlen(options[i]) / 2, "%s",
               options[i]);

      if (i == chosen) {
        attroff(A_BOLD | A_REVERSE);
      }
    }

    refresh();
    key = getch();
    if (key == 'p') {
      paused = false;
      return SO_RESUME;
    }
  }
}
