#include "colors.h"
#include "game.h"
#include "leaderboard.h"
#include "optionmenu.h"
#include "startscreen.h"
#include <locale.h>
#include <ncurses.h>
#include <signal.h>
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

// special global variables for pages
static OptionMenu g_pausemenu = {0};
static OptionMenu g_endgamemenu = {0};
static Score g_leaderboard[SCORESIZ] = {0};

void cnake_exit() {
  clear();
  refresh();
  endwin();
  exit(0);
}

const char *cnake_get_cache_dir(void) {
  const char *path = getenv("CSIDL_LOCAL_APPDATA");
  if (!path) {
    return path;
  } else {
    return NULL;
  }

  return "~/.cache/";
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

void start_game(bool multiplayer) {
  int key = 0, maxy = 0, maxx = 0;
  getmaxyx(stdscr, maxy, maxx);

  Snake snake1 = snake_new(maxy, maxx, PLAYER_ONE);
  Snake snake2;
  // pretend that snake2 is always alive when in single player
  // this allows me to only have to handle death once
  // as as long as one of the snakes is dead the game ends
  snake2.is_alive = true;

  if (multiplayer) {
    snake2 = snake_new(maxy, maxx, PLAYER_TWO);
  }

  Food food = food_new(&snake1, maxy, maxx);
  int score1 = 0, score2 = 0;

  for (;;) {
    erase();
    getmaxyx(stdscr, maxy, maxx);
    maxy -= 2;
    snake_next_frame(maxy, maxx, key, &score1, &snake1, &food);

    if (multiplayer) {
      snake_next_frame(maxy, maxx, key, &score2, &snake2, &food);
      // the first item in scoreboard is always the biggest
      score_draw(2, score1, score2, g_leaderboard[0].score, maxy + 1, maxx);

      snake_check_collision(&snake1, &snake2);
    } else {
      score_draw(1, score1, score2, g_leaderboard[0].score, maxy + 1, maxx);
    }

    switch (optionmenu_draw(&g_pausemenu, key, maxy / 2, maxx / 2)) {
    case SO_MAINMENU:
      return;
      break;

    case SO_RESTART:
    restart:
      snake_free(&snake1);
      snake1 = snake_new(maxy, maxx, PLAYER_ONE);
      score1 = 0;

      if (multiplayer) {
        snake_free(&snake2);
        snake2 = snake_new(maxy, maxx, PLAYER_TWO);
        score2 = 0;
      }

      food = food_new(&snake1, maxy, maxx);
      break;

    case SO_QUIT:
      cnake_exit();
      break;

    default:
      break;
    }

    if (!snake1.is_alive || !snake2.is_alive) {
      if (multiplayer) {
        if (snake1.is_alive) {
          optionmenu_change_title(&g_endgamemenu, "Player 1 Won");
        } else if (snake2.is_alive) {
          optionmenu_change_title(&g_endgamemenu, "Player 2 Won");
        }
      }
      snake1.is_alive = false;
      snake2.is_alive = false;
      leaderboard_add_score(g_leaderboard, "AAA", score1);
      leaderboard_add_score(g_leaderboard, "AAA", score2);
      // TODO: remove score.bin and have proper filepath resolution
      leaderboard_save(g_leaderboard, "./score.bin");

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
    refresh();
  }

  snake_free(&snake1);
  if (multiplayer) {
    snake_free(&snake2);
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
  timeout(5);
  cnake_init_colors();
  // cleanup and exit if the program is interrupted/terminated
  signal(SIGINT, cnake_exit);
  signal(SIGTERM, cnake_exit);

  g_pausemenu = optionmenu_new("PAUSED", true);
  optionmenu_add_option(&g_pausemenu, SO_RESUME, "Resume");
  optionmenu_add_option(&g_pausemenu, SO_RESTART, "Restart");
  optionmenu_add_option(&g_pausemenu, SO_MAINMENU, "Main Menu");
  optionmenu_add_option(&g_pausemenu, SO_QUIT, "Quit");

  g_endgamemenu = optionmenu_new("GAME OVER", false);
  optionmenu_add_option(&g_endgamemenu, SO_RESTART, "Restart");
  optionmenu_add_option(&g_endgamemenu, SO_MAINMENU, "Main Menu");
  optionmenu_add_option(&g_endgamemenu, SO_QUIT, "Quit");

  // TODO: remove score.bin and have proper filepath resolution
  leaderboard_load(g_leaderboard, "./score.bin");

  int maxy = 0, maxx = 0;
  int key = 0;
  for (;;) {
    erase();
    getmaxyx(stdscr, maxy, maxx);
    switch (draw_startscreen(key, maxy, maxx)) {
    case SP_ONE_PLAYER:
      start_game(false);
      break;
    case SP_TWO_PLAYERS:
      start_game(true);
      break;
    case SP_LEADERBOARD:
      for (;;) {
        leaderboard_draw(g_leaderboard, maxy, maxx);
        key = getch();
        if (key == '\n' || key == 'q' || key == 'p') {
          break;
        }
      }
      break;
    case SP_NONE:
      break;
    }

    refresh();
    key = getch();
  }
  return 0;
}
