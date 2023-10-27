#include "pausemenu.h"
#include "colors.h"
#include "vector.h"
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

OptionMenu optionmenu_new(char *title, bool hideable) {
  OptionMenu om = {
      .chosen = 0,
      .options = vec_new(sizeof(OptionField)),
      .hideable = hideable,
  };

  if (hideable) {
    om.paused = false;
  } else {
    om.paused = true;
  }

  strcpy(om.title, title);
  return om;
}

void optionmenu_add_option(OptionMenu *self, unsigned int id, char *fieldname) {
  OptionField of;
  of.id = id;
  strcpy(of.field, fieldname);
  vec_push(self->options, &of);
}

// TODO: make selected options all be the same width
unsigned int optionmenu_draw(OptionMenu *self, int key, int y, int x) {
  if (self->hideable) {
    if (key == 'p') {
      self->paused = !self->paused;
    }

    if (!self->paused) {
      return optionmenu_HIDE;
    }
  }

  size_t options_size = vec_len(self->options);

  for (;;) {
    // --- menu actions ---
    switch (key) {
    case 'j':
    case 's':
    case KEY_DOWN:
      if (self->chosen < options_size - 1) {
        ++self->chosen;
      }
      break;

    case 'k':
    case 'w':
    case KEY_UP:
      if (self->chosen > 0) {
        --self->chosen;
      }
      break;

    case '\n':
      if (self->hideable) {
        self->paused = false;
      }
      OptionField of;
      vec_get(self->options, self->chosen, &of);
      return of.id;
    }

    // --- draw menu ---
    attron(A_BOLD);
    mvprintw(y - 2 - options_size / 2, x - strlen(self->title) / 2,
             self->title);
    attroff(A_BOLD);
    for (int i = 0; i < options_size; i++) {
      // highlight selected option on menu
      if (i == self->chosen) {
        attron(A_BOLD | A_REVERSE);
      }

      OptionField of;
      vec_get(self->options, i, &of);
      mvprintw(y + i - options_size / 2, x - strlen(of.field) / 2, "%s",
               of.field);

      if (i == self->chosen) {
        attroff(A_BOLD | A_REVERSE);
      }
    }

    refresh();

    // --- listen to user input ---
    key = getch();
    if (self->hideable && key == 'p') {
      self->paused = false;
      return optionmenu_SHOW;
    }
  }
}
