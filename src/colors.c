#include <ncurses.h>

void cnake_init_colors(void) {
  // only use colors if terminal supports it
  if (has_colors()) {
    start_color();
    use_default_colors();
    // initialize color pairs
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
  }
}
