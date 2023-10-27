#ifndef PAUSE_H
#define PAUSE_H

#include "vector.h"
#include <stddef.h>

#define FIELD_MAXSIZE 25

typedef enum optionmenu_state {
  optionmenu_SHOW = -1,
  optionmenu_HIDE = -2,
} OptionMenuState;

typedef struct option_menu {
  char title[FIELD_MAXSIZE];
  bool hideable;
  size_t chosen;
  bool paused;
  vec_Vector *options;
} OptionMenu;

typedef struct option_field {
  char field[FIELD_MAXSIZE];
  int id;
} OptionField;

// creates a optionmenu object
//
// title: the title shown on the optionmenu
// hideable: whether the menu can be hid by pressing `p`
//
// hideable menus won't render if it's state is marked as hide
// whereas unhideable ones will always show
OptionMenu optionmenu_new(char *title, bool hideable);

// adds an option to the menu as by default there's no options
//
// id: an enum value that should be returns when `fieldname` is chosen
void optionmenu_add_option(OptionMenu *self, unsigned int id, char *fieldname);

unsigned int optionmenu_draw(OptionMenu *self, int key, int y, int x);

#endif
