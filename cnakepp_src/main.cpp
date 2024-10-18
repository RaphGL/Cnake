#include "food.hpp"
#include "game_manager.hpp"
#include "snake.hpp"
#include <chrono>
#include <clocale>
#include <ncurses.h>

namespace game {
float delta{};
}

int main() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, true);

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

  std::chrono::high_resolution_clock clock{};
  auto t1{clock.now()};
  auto t2{t1};
  for (;;) {
    t1 = clock.now();

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

    for (;;) {
      t2 = clock.now();
      auto microseconds{
          std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
              .count()};

      if (microseconds > 8000) {
        game::delta =
            static_cast<decltype(game::delta)>(microseconds) / 1000'000;
        break;
      }
    }

    wrefresh(gwin);
  }
}
