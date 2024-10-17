#include "game_manager.hpp"
#include "snake.hpp"
#include <ncurses.h>

GameManager::GameManager() {
  const auto pos = this->get_centered_position();
  m_game_win = newwin(game_height, game_width, pos.y, pos.x);
  m_panel_win = newwin(1, game_width, pos.y + game_height, pos.x);
  // todo: change to using delta for a more consistent game framerate
  wtimeout(m_game_win, 100);
}

GameManager::~GameManager() { delwin(m_game_win); }

Coordinate GameManager::get_centered_position() {
  int maxy, maxx;
  getmaxyx(stdscr, maxy, maxx);

  return (Coordinate){
      .x = maxx / 2 - game_width / 2,
      .y = maxy / 2 - game_height / 2,
  };
}

void GameManager::draw_panel() {
  constexpr auto attribs{A_REVERSE | A_BOLD};
  wattron(m_panel_win, attribs);
  for (std::size_t i = 0; i < game_width; i++) {
    wprintw(m_panel_win, " ");
  }
  mvwprintw(m_panel_win, 0, 4, "Score: %d", m_score_1);
  wattroff(m_panel_win, attribs);
  wrefresh(m_panel_win);
}

void GameManager::adjust_position() {
  const auto new_pos = this->get_centered_position();
  werase(m_game_win);
  werase(m_panel_win);
  wrefresh(m_game_win);
  wrefresh(m_panel_win);
  mvwin(m_game_win, new_pos.y, new_pos.x);
  mvwin(m_panel_win, new_pos.y + game_height, new_pos.x);
}

void GameManager::tick() {
  wborder(m_game_win, 0, 0, 0, 0, 0, 0, 0, 0);
  this->draw_panel();
}

void GameManager::add_point_to(Player num) {
  switch (num) {
  case Player::One:
    ++m_score_1;
    break;
  case Player::Two:
    ++m_score_2;
    break;
  case Player::None:
    // do nothing
    break;
  }
}

WINDOW *GameManager::get_win() const { return m_game_win; }
