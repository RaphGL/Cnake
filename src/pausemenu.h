#ifndef PAUSE_H
#define PAUSE_H

#include <stdbool.h>

typedef enum {
  SO_RESUME,
  SO_RESTART,
  SO_MAINMENU,
  SO_QUIT,
  SO_NONE
} SelectedOption;

SelectedOption pausemenu_draw(int key, int y, int x);

#endif
