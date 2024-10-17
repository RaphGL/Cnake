#include "snake.hpp"
#include "colors.hpp"
#include <ncurses.h>

constexpr char SNAKE_BLOCK[] = "█";

Snake::Snake(WINDOW *win, Player num) : m_player_num{num}, m_win{win} {
  int maxy, maxx;
  getmaxyx(m_win, maxy, maxx);
  m_y = maxy / 2;

  constexpr std::size_t default_snake_len = 4;

  if (m_player_num == Player::One) {
    m_x = maxx / 2 - maxx / default_snake_len;
    m_direction = Direction::Right;
    for (int i = default_snake_len; i > 0; i--) {
      m_body.push_back((Coordinate){.x = m_x - i, .y = m_y});
    }
  }

  if (m_player_num == Player::Two) {
    m_x = maxx / 2 + maxx / default_snake_len;
    m_direction = Direction::Left;
    for (int i = default_snake_len; i > 0; i--) {
      m_body.push_back((Coordinate){.x = m_x + i, .y = m_y});
    }
  }
}

void Snake::choose_direction_from_input(int key) {
  auto choose_direction = [&](int up, int down, int left, int right) {
    if (key == up) {
      if (m_direction == Direction::Down) {
        return;
      }
      m_direction = Direction::Up;

    } else if (key == left) {
      if (m_direction == Direction::Right) {
        return;
      }
      m_direction = Direction::Left;

    } else if (key == down) {
      if (m_direction == Direction::Up) {
        return;
      }
      m_direction = Direction::Down;

    } else if (key == right) {
      if (m_direction == Direction::Left) {
        return;
      }
      m_direction = Direction::Right;
    }
  };

  if (m_player_num == Player::One) {
    choose_direction('w', 's', 'a', 'd');
  }

  if (m_player_num == Player::Two) {
    choose_direction(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
  }
}

void Snake::check_health() {
  for (const auto part : m_body) {
    if (part.x == m_x && part.y == m_y) {
      m_is_alive = false;
      break;
    }
  }
}

void Snake::advance_in_direction() {
  this->check_health();
  m_body.push_back((Coordinate){.x = m_x, .y = m_y});
  m_body.pop_front();

  switch (m_direction) {
  case Direction::Left:
    --m_x;
    break;

  case Direction::Right:
    ++m_x;
    break;

  case Direction::Up:
    --m_y;
    break;

  case Direction::Down:
    ++m_y;
    break;
  }

  int maxx, maxy;
  getmaxyx(m_win, maxy, maxx);

  if (m_x >= maxx - 1) {
    m_x = 1;
  }

  if (m_x < 1) {
    m_x = maxx - 2;
  }

  if (m_y >= maxy - 1) {
    m_y = 1;
  }

  if (m_y < 1) {
    m_y = maxy - 2;
  }
}

void Snake::tick(int key) {
  if (m_is_alive) {
    this->choose_direction_from_input(key);
    this->advance_in_direction();
  }

  // body
  unsigned int player_color{};
  if (m_player_num == Player::One) {
    player_color = GREEN_FG;
  } else if (m_player_num == Player::Two) {
    player_color = YELLOW_FG;
  }

  wattron(m_win, player_color);
  for (const auto part : m_body) {
    mvwprintw(m_win, part.y, part.x, "%s", SNAKE_BLOCK);
  }
  wattroff(m_win, player_color);

  // head
  if (!m_is_alive) {
    wattron(m_win, RED_FG);
    auto head = m_body.back();
    mvwprintw(m_win, head.y, head.x, SNAKE_BLOCK);
    wattroff(m_win, RED_FG);
  } else {
    mvwprintw(m_win, m_y, m_x, SNAKE_BLOCK);
  }
}

void Snake::eat() {
  // reinserts a preexisting body part cause if the head were to
  // be reinserted the head would overlap with a body causing the game
  // to think that the snake is dead
  auto part = m_body.back();
  m_body.push_back(part);
}
