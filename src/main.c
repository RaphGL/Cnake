#include "colors.h"
#include "game.h"
#include "startscreen.h"
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h> // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

// cross-platform sleep function
void sleep_ms(int milliseconds) {
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

void start_one_player() {
  int key = 0, maxy = 0, maxx = 0;
  getmaxyx(stdscr, maxy, maxx);

  Snake snake = snake_new(maxy, maxx);
  Food food = food_new(&snake, maxy, maxx);
  int score = 0;
  for (;;) {
    clear();
    getmaxyx(stdscr, maxy, maxx);
    maxy -= 2;

    snake_draw(&snake, key, maxy, maxx);
    food_draw(&food);
    score_draw(score, maxy + 1, maxx);

    if (food_is_eaten(&food, &snake)) {
      snake_eat(&snake, &food);
      food = food_new(&snake, maxy, maxx);
      ++score;
    }

    if (!snake.is_alive) {
      exit(0);
    }

    key = getch();
    refresh();
  }

  snake_free(&snake);
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
  timeout(80);
  cnake_init_colors();

  int maxy = 0, maxx = 0;
  int key = 0;
  for (;;) {
    clear();
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
