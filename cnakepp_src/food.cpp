#include "food.hpp"
#include "snake.hpp"
#include <ncurses.h>
#include <random>

namespace {
  std::random_device rand_dev{};
  std::mt19937 mt{rand_dev()};
}

constexpr auto FOOD_BLOCK{"■"};

Food::Food(WINDOW *win, Snake *snake1, Snake *snake2) : m_win{win} {
  int maxy, maxx;
  getmaxyx(m_win, maxy, maxx);
  std::uniform_int_distribution<int> rand_x{1, maxx - 2};
  std::uniform_int_distribution<int> rand_y{1, maxy - 2};

  // max* - 2 is the maximum position under a boxed window
  // 1 is the minimum position under a boxed window

  auto found_position = [this](Snake *snake) {
    // found position where the snake body is not occupying
    bool found{true};
    for (const auto part : snake->m_body) {
      if (m_x == part.x && m_y == part.y) {
        return false;
      }
    }

    if (m_x == snake->m_x && m_y == snake->m_y) {
      return false;
    }

    return true;
  };

  for (;;) {
    m_x = rand_x(mt);
    m_y = rand_y(mt);

    if (snake2 != nullptr) {
      if (found_position(snake1) && found_position(snake2)) {
        break;
      } else {
        continue;
      }
    } else if (found_position(snake1)) {
      break;
    }
  }
}

void Food::tick() { mvwprintw(m_win, m_y, m_x, FOOD_BLOCK); }

Player Food::is_eaten(Snake *snake1, Snake *snake2) {
  if (snake1->m_x == m_x && snake1->m_y == m_y) {
    return Player::One;
  }

  if (snake2 != nullptr && snake2->m_x == m_x && snake2->m_y == m_y) {
    return Player::Two;
  }

  return Player::None;
}
