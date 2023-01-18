#include "food.hpp"
#include <array>
#include <ncurses.h>
#include <random>
#include <time.h>
#include <vector>

Food::Food(std::vector<std::array<int, 2>> &snake_body)
{
  int maxx, maxy;
  getmaxyx(stdscr, maxy, maxx);

  for (;;)
  {
    std::srand(time(NULL));
    this->x = std::rand() % (maxx - 3);
    this->y = std::rand() % (maxy - 4 - 3);

    // makes loop break if a new food position is found that doesn't overlap
    // with the snake body
    bool found_position = true;
    for (const auto &i : snake_body) // i[0] == snake.x && i[1] == snake.y
    {
      if (this->x == i.at(0) && this->y == i.at(1))
      {
        found_position = false;
        break;
      }
    }

    if (found_position && this->x > 3 && this->y > 3)
    {
      break;
    }
  }

  mvprintw(this->y, this->x, "▒");
}

void Food::draw() { mvprintw(this->y, this->x, "▒"); }

bool Food::is_eaten(int snake_y, int snake_x)
{
  if (this->x == snake_x && this->y == snake_y)
  {
    mvprintw(this->y, this->x, "█");
    return true;
  }

  return false;
}
