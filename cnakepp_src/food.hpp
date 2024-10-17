#ifndef FOOD_HPP
#define FOOD_HPP

#include "snake.hpp"
#include <ncurses.h>

class Food {
  int m_x, m_y;
  WINDOW *m_win{nullptr};

public:
  explicit Food(WINDOW *win, Snake *const snake1, Snake *const snake2 = nullptr);
  void tick();
  Player is_eaten(Snake *snake1, Snake *snake2 = nullptr);
};

#endif
