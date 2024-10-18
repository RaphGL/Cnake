#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>

namespace game {

// delta calculated by the gameplay look
// it is global to the entire program (defined on main.cpp)
extern float delta;

constexpr int WIDTH = 60;
constexpr int HEIGHT = 22;

} // namespace game

namespace color {

constexpr auto GREEN_FG{COLOR_PAIR(1)};
constexpr auto GREEN_BG{COLOR_PAIR(2)};
constexpr auto YELLOW_FG{COLOR_PAIR(3)};
constexpr auto RED_FG{COLOR_PAIR(4)};

} // namespace color

#endif
