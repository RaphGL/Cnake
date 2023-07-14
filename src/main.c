#include "colors.h"
#include "startscreen.h"
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h> // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds) { // cross-platform sleep function
#ifdef WIN32
  Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&ts, NULL);
#else
  if (milliseconds >= 1000)
    sleep(milliseconds / 1000);
  usleep((milliseconds % 1000) * 1000);
#endif
}

void start_one_player(void) {
  clear();
  for (;;) {
    mvprintw(0, 0, "start one player");
    refresh();
  }
}

void start_two_players(void) {
  clear();
  for (;;) {
    mvprintw(0, 0, "start two players");
    refresh();
  }
}

void draw_leaderboard(void) {
  clear();
  for (;;) {
    mvprintw(0, 0, "show leaderboard");
    refresh();
  }
}

int main(void) {
  clear();
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  cnake_init_colors();

  int maxy = 0, maxx = 0;
  int key = 0;
  for (;;) {
    getmaxyx(stdscr, maxy, maxx);
    switch (draw_startscreen(key, maxy, maxx)) {
    case ONE_PLAYER:
      start_one_player();
      break;
    case TWO_PLAYERS:
      start_two_players();
      break;
    case LEADERBOARD:
      draw_leaderboard();
      break;
    case NONE:
      break;
    }

    refresh();
    key = getch();
    sleep_ms(1 / 60);
  }
  return 0;
}
