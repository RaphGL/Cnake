#include "colors.h"
#include "game.h"
#include "pausemenu.h"
#include "startscreen.h"
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
  SO_RESUME,
  SO_RESTART,
  SO_MAINMENU,
  SO_QUIT,
} SelectedOption;

static time_t g_start = 0, g_end = 0;

static OptionMenu g_pausemenu = {0};
static OptionMenu g_endgamemenu = {0};

void wait_30fps_passed(void) {
  for (;;) {
    time(&g_end);
    if (g_end - g_start >= 1 / 30)
      break;
  }
}

void cnake_exit() {
  clear();
  refresh();
  endwin();
  exit(0);
}

void snake_next_frame(int maxy, int maxx, int key, int *score, Snake *snake,
                      Food *food) {
  snake_draw(snake, key, maxy, maxx);
  food_draw(food);

  if (food_is_eaten(food, snake)) {
    snake_eat(snake, food);
    *food = food_new(snake, maxy, maxx);
    ++(*score);
  }
}

void start_one_player(void) {
  int key = 0, maxy = 0, maxx = 0;
  getmaxyx(stdscr, maxy, maxx);

  Snake snake = snake_new(maxy, maxx, 1);
  Food food = food_new(&snake, maxy, maxx);
  int score = 0;
  for (;;) {
    time(&g_start);
    clear();
    getmaxyx(stdscr, maxy, maxx);
    maxy -= 2;
    snake_next_frame(maxy, maxx, key, &score, &snake, &food);
    score_draw(1, score, 0, maxy + 1, maxx);

    switch (optionmenu_draw(&g_pausemenu, key, maxy / 2, maxx / 2)) {
    case SO_MAINMENU:
      return;
      break;

    case SO_RESTART:
    restart:
      snake_free(&snake);
      snake = snake_new(maxy, maxx, 1);
      food = food_new(&snake, maxy, maxx);
      score = 0;
      break;

    case SO_QUIT:
      cnake_exit();
      break;

    default:
      break;
    }

    if (!snake.is_alive) {
      for (;;) {
        key = getch();
        switch (optionmenu_draw(&g_endgamemenu, key, maxy / 2, maxx / 2)) {
        case SO_QUIT:
          cnake_exit();
          break;

        case SO_RESTART:
          snake.is_alive = true;
          goto restart;
          break;

        case SO_MAINMENU:
          return;
          break;
        }
      }
    }

    key = getch();
    wait_30fps_passed();
    refresh();
  }

  snake_free(&snake);
}

void start_two_players(void) {
  int key = 0, maxy = 0, maxx = 0;
  getmaxyx(stdscr, maxy, maxx);

  Snake snake1 = snake_new(maxy, maxx, PLAYER_ONE);
  Snake snake2 = snake_new(maxy, maxx, PLAYER_TWO);
  snake2.y += 5;
  Food food = food_new(&snake1, maxy, maxx);
  int score1 = 0, score2 = 0;

  for (;;) {
    time(&g_start);
    clear();
    getmaxyx(stdscr, maxy, maxx);
    maxy -= 2;
    snake_next_frame(maxy, maxx, key, &score1, &snake1, &food);
    snake_next_frame(maxy, maxx, key, &score2, &snake2, &food);
    score_draw(2, score1, score2, maxy + 1, maxx);

    switch (optionmenu_draw(&g_pausemenu, key, maxy / 2, maxx / 2)) {
    case SO_MAINMENU:
      return;
      break;

    case SO_RESTART:
    restart:
      snake_free(&snake1);
      snake_free(&snake2);
      snake1 = snake_new(maxy, maxx, PLAYER_ONE);
      snake2 = snake_new(maxy, maxx, PLAYER_TWO);
      snake2.y += 5;
      score1 = 0;
      score2 = 0;
      food = food_new(&snake1, maxy, maxx);
      break;

    case SO_QUIT:
      cnake_exit();
      break;

    default:
      break;
    }

    // TODO: say which player lost by colliding
    switch (snake_collided(&snake1, &snake2)) {
    case PLAYER_ONE:
      snake2.is_alive = false;
      break;
    case PLAYER_TWO:
      snake1.is_alive = false;
      break;
    case PLAYER_NONE:
      break;
    }

    if (!snake1.is_alive || !snake2.is_alive) {
      snake1.is_alive = false;
      snake2.is_alive = false;
      for (;;) {
        key = getch();
        switch (optionmenu_draw(&g_endgamemenu, key, maxy / 2, maxx / 2)) {
        case SO_QUIT:
          cnake_exit();
          break;

        case SO_RESTART:
          snake1.is_alive = true;
          snake2.is_alive = true;
          goto restart;
          break;

        case SO_MAINMENU:
          return;
          break;
        }
      }
    }

    key = getch();
    wait_30fps_passed();
    refresh();
  }

  snake_free(&snake1);
  snake_free(&snake2);
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

  g_pausemenu = optionmenu_new("PAUSED", true);
  optionmenu_add_option(&g_pausemenu, SO_RESUME, "Resume");
  optionmenu_add_option(&g_pausemenu, SO_RESTART, "Restart");
  optionmenu_add_option(&g_pausemenu, SO_MAINMENU, "Main Menu");
  optionmenu_add_option(&g_pausemenu, SO_QUIT, "Quit");

  g_endgamemenu = optionmenu_new("GAME OVER", false);
  optionmenu_add_option(&g_endgamemenu, SO_RESTART, "Restart");
  optionmenu_add_option(&g_endgamemenu, SO_MAINMENU, "Main Menu");
  optionmenu_add_option(&g_endgamemenu, SO_QUIT, "Quit");

  int maxy = 0, maxx = 0;
  int key = 0;
  for (;;) {
    clear();
    getmaxyx(stdscr, maxy, maxx);
    switch (draw_startscreen(key, maxy, maxx)) {
    case SP_ONE_PLAYER:
      start_one_player();
      break;
    case SP_TWO_PLAYERS:
      start_two_players();
      break;
    case SP_LEADERBOARD:
      // TODO
      break;
    case SP_NONE:
      break;
    }

    refresh();
    key = getch();
  }
  return 0;
}
