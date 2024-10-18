#include "game_manager.hpp"
#include "consts.hpp"
#include "snake.hpp"
#include <format>
#include <ncurses.h>

GameManager::GameManager() {
  const auto pos = this->get_centered_position();
  m_game_win = newwin(game::HEIGHT, game::WIDTH, pos.y, pos.x);
  m_panel_win = newwin(1, game::WIDTH, pos.y + game::HEIGHT, pos.x);
  wtimeout(m_game_win, 1);
}

GameManager::~GameManager() { delwin(m_game_win); }

Coordinate GameManager::get_centered_position() {
  float maxy, maxx;
  getmaxyx(stdscr, maxy, maxx);

  Coordinate coord = {
      .x = maxx / 2 - static_cast<float>(game::WIDTH) / 2,
      .y = maxy / 2 - static_cast<float>(game::HEIGHT) / 2,
  };

  return coord;
}

void GameManager::draw_panel() {
  constexpr auto attribs{A_REVERSE | A_BOLD};
  wattron(m_panel_win, attribs);

  for (std::size_t i = 0; i < game::WIDTH; i++) {
    wprintw(m_panel_win, " ");
  }
  auto panel_msg = std::format("Score: {}   Best: {}", m_score_1, 0);
  mvwprintw(m_panel_win, 0, game::WIDTH / 2 - panel_msg.size() / 2, "%s",
            panel_msg.c_str());

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
  mvwin(m_panel_win, new_pos.y + game::HEIGHT, new_pos.x);
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
