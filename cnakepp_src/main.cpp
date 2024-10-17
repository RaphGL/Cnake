#include "food.hpp"
#include "snake.hpp"
#include "game_manager.hpp"
#include <clocale>
#include <ncurses.h>


int main() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  timeout(5);

  if (has_colors()) {
    start_color();
    use_default_colors();
    // initialize color pairs
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_RED, -1);
  }

  GameManager game_manager{};
  Snake snake{game_manager.get_win(), Player::One};
  Food food{game_manager.get_win(), &snake};

  for (;;) {
    const auto gwin = game_manager.get_win();
    int key = wgetch(gwin);
    werase(gwin);

    if (key == KEY_RESIZE) {
      game_manager.adjust_position();
    }

    game_manager.tick();
    food.tick();
    snake.tick(key);

    if (auto player = food.is_eaten(&snake); player != Player::None) {
      food = Food(gwin, &snake);
      snake.eat();
      game_manager.add_point_to(player);
    }

    wrefresh(gwin);
  }
}
