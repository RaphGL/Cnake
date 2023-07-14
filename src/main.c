#include "colors.h"
#include "startscreen.h"
#include <locale.h>
#include <ncurses.h>
#include <string.h>

int main(void) {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  cnake_init_colors();

  int maxy, maxx;
  getmaxyx(stdscr, maxy, maxx);

  for (;;) {
    int key = getch();
  }

  draw_startscreen(maxy, maxx);
  getch();
  return 0;
}
