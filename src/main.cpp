#include "bar.hpp"
#include "food.hpp"
#include "snake.hpp"
#include <array>
#include <csignal>
#include <ncurses.h>
#include <random>

int main()
{
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(TIMEOUT);
  curs_set(false);

  int score = 0;
  Bar bar{};
  bar.draw(score);

  Snake snake{};
  Food food(snake.body);

  signal(SIGINT, [](int signum)
         {
    endwin();
    exit(signum); });

  for (;;)
  {
    snake.update();
    food.draw();

    if (food.is_eaten(snake.y, snake.x))
    {
      score++;
      bar.draw(score);
      food = Food(snake.body);
      snake.grow();
      snake.grow();
    }

    if (!snake.is_alive)
    {
      bar.show_endscreen(score);
    }

    refresh();
  }
  return 0;
}
