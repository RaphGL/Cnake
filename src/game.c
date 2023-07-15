#include "game.h"
#include "colors.h"
#include "vector.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Snake snake_new(int maxy, int maxx) {
  vec_Vector *vec = vec_new(sizeof(Coordinate));
  if (!vec) {
    fputs("Could not allocate memory for snake body.", stderr);
    exit(1);
  }

  vec_push(vec, &(Coordinate){.x = maxx / 2 - 2, .y = maxy / 2});
  vec_push(vec, &(Coordinate){.x = maxx / 2 - 1, .y = maxy / 2});
  vec_push(vec, &(Coordinate){.x = maxx / 2, .y = maxy / 2});
  return (Snake){
      .body_coords = vec,
      .direction = RIGHT,
      .x = maxx / 2,
      .y = maxy / 2,
  };
}

void snake_free(Snake *self) {
  vec_free(self->body_coords);
  memset(self, 0, sizeof(*self));
}

static void snake_get_direction(Snake *self, int key) {
  switch (key) {
  case 'w':
  case KEY_UP:
    if (self->direction == DOWN) {
      return;
    }
    self->direction = UP;
    break;
  case 's':
  case KEY_DOWN:
    if (self->direction == UP) {
      return;
    }
    self->direction = DOWN;
    break;
  case 'd':
  case KEY_RIGHT:
    if (self->direction == LEFT) {
      return;
    }
    self->direction = RIGHT;
    break;
  case 'a':
  case KEY_LEFT:
    if (self->direction == RIGHT) {
      return;
    }
    self->direction = LEFT;
    break;
  }
}

#define TIMEOUT 80
static void snake_update(Snake *self, int maxy, int maxx) {
  // this->check_life_status();
  //
  // if (!is_alive) {
  //   return;
  // }

  switch (self->direction) {
  case LEFT:
    timeout(TIMEOUT);
    self->x--;
    break;
  case RIGHT:
    timeout(TIMEOUT);
    self->x++;
    break;
  case UP:
    timeout(TIMEOUT * 1.8);
    self->y--;
    break;
  case DOWN:
    timeout(TIMEOUT * 1.8);
    self->y++;
    break;
  }

  if (self->x > maxx) {
    self->x = 0;
  }

  if (self->x < 0) {
    self->x = maxx;
  }

  if (self->y > maxy) {
    self->y = 0;
  }

  if (self->y < 0) {
    self->y = maxy;
  }
}

void snake_draw(Snake *const self, int key, int maxy, int maxx) {
  attron(GREEN_FG);
  snake_get_direction(self, key);
  snake_update(self, maxy, maxx);
  vec_push(self->body_coords, &(Coordinate){.x = self->x, .y = self->y});
  vec_remove(self->body_coords, 0, NULL);

  for (size_t i = 0; i < vec_len(self->body_coords); i++) {
    Coordinate coord = {0};
    vec_get(self->body_coords, i, &coord);
    mvprintw(coord.y, coord.x, "█");
  }

  attroff(GREEN_FG);
}

void snake_grow(Snake *self) {
  Coordinate curr = (Coordinate){.x = self->x, .y = self->y};
  vec_push(self->body_coords, &curr);
}

Food food_new(Snake *const snake, int maxy, int maxx) {
  Food food;
  srand(time(NULL));

  for (;;) {
    food.x = rand() % maxx;
    food.y = rand() % maxy;

    // makes loop break if a new food position is found that doesn't overlap
    // with the snake body
    bool found_position = true;
    for (size_t i = 0; i < vec_len(snake->body_coords); i++) {
      Coordinate curr;
      vec_get(snake->body_coords, i, &curr);

      if (food.x == curr.x && food.y == curr.y) {
        found_position = false;
        break;
      }
    }

    if (found_position) {
      break;
    }
  }

  return food;
}

void food_draw(Food *self) { mvprintw(self->y, self->x, "■"); }

bool food_is_eaten(Food *self, Snake *const snake) {
  if (self->x == snake->x && self->y == snake->y) {
    mvprintw(self->y, self->x, "█");
    return true;
  }

  return false;
}

void score_draw(int score, int maxy, int maxx) {
  move(maxy, 0);
  for (int i = 0; i < maxx; i++) {
    printw("█");
  }

  attron(A_REVERSE | A_BOLD);
  char msg[100] = {0};
  snprintf(msg, sizeof(msg), "Score %d    Best: %d", score, 0);
  mvprintw(maxy, (maxx - strlen(msg)) / 2, "%s", msg);
  attroff(A_REVERSE | A_BOLD);
}
