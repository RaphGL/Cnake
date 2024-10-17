#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <ncurses.h>
#include "snake.hpp"

class GameManager {
  WINDOW *m_game_win{nullptr};
  WINDOW *m_panel_win{nullptr};
  static constexpr int game_width = 60;
  static constexpr int game_height = 22;
  int m_score_1{0};
  int m_score_2{0};

  static Coordinate get_centered_position();
  void draw_panel();

public:
  explicit GameManager();
  ~GameManager();

  void adjust_position();
  void tick();
  void add_point_to(Player num);
  WINDOW *get_win() const;
};

#endif
