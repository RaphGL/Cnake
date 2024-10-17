#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <deque>
#include <ncurses.h>

enum class Direction { Left, Right, Up, Down };

enum class Player { None, One, Two };

struct Coordinate {
  int x, y;
};

class Snake {
  friend class Food;

  std::deque<Coordinate> m_body{};
  int m_x, m_y;
  bool m_is_alive{true};
  Player m_player_num{Player::One};
  Direction m_direction;
  WINDOW *m_win{nullptr};

  void choose_direction_from_input(int key);
  // checks whether snake bit its own body and then updates m_is_alive
  void check_health();
  // advance snake head to the next position in the direction
  void advance_in_direction();

public:
  explicit Snake(WINDOW *win, Player num);
  // render one time frame
  void tick(int key);
  void eat();
};

#endif
