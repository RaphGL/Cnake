#include "game.h"
#include "colors.h"
#include "vector.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Snake snake_new(int maxy, int maxx, Player player_no) {
  vec_Vector *snake_body = vec_new(sizeof(Coordinate));
  if (!snake_body) {
    fputs("Could not allocate memory for snake body.", stderr);
    exit(1);
  }

  // creates snake with size 4
  // the head is stored as snake.x and snake.y
  // whereas the rest of the body is in the body_coords vector
  for (size_t i = 4; i > 0; i--) {
    vec_push(snake_body, &(Coordinate){.x = maxx / 2 - i, .y = maxy / 2});
  }

  return (Snake){
      .body_coords = snake_body,
      .direction = RIGHT,
      .x = maxx / 2,
      .y = maxy / 2,
      .player_no = player_no,
      .is_alive = true,
  };
}

void snake_free(Snake *self) {
  vec_free(self->body_coords);
  memset(self, 0, sizeof(*self));
}

static void snake_get_direction(Snake *self, int key) {
  if (self->player_no == PLAYER_ONE) {
    switch (key) {
    case 'w':
      if (self->direction == DOWN) {
        return;
      }
      self->direction = UP;
      break;
    case 's':
      if (self->direction == UP) {
        return;
      }
      self->direction = DOWN;
      break;
    case 'd':
      if (self->direction == LEFT) {
        return;
      }
      self->direction = RIGHT;
      break;
    case 'a':
      if (self->direction == RIGHT) {
        return;
      }
      self->direction = LEFT;
      break;
    }
  }

  if (self->player_no == PLAYER_TWO) {
    switch (key) {
    case KEY_UP:
      if (self->direction == DOWN) {
        return;
      }
      self->direction = UP;
      break;
    case KEY_DOWN:
      if (self->direction == UP) {
        return;
      }
      self->direction = DOWN;
      break;
    case KEY_RIGHT:
      if (self->direction == LEFT) {
        return;
      }
      self->direction = RIGHT;
      break;
    case KEY_LEFT:
      if (self->direction == RIGHT) {
        return;
      }
      self->direction = LEFT;
      break;
    }
  }
}

// checks for collision between head and some other body part
// and updates self.is_alive
static inline void snake_update_health_status(Snake *self) {
  Coordinate curr;
  for (size_t i = 0; i < vec_len(self->body_coords); i++) {
    vec_get(self->body_coords, i, &curr);
    if (curr.x == self->x && curr.y == self->y) {
      self->is_alive = false;
    }
  }
}

#define TIMEOUT 80
static void snake_update(Snake *self, int maxy, int maxx) {
  // check should happen before adding old head to vector
  // otherwise this might result in conflicts when snake grows
  snake_update_health_status(self);
  vec_push(self->body_coords, &(Coordinate){.x = self->x, .y = self->y});
  vec_remove(self->body_coords, 0, NULL);

  switch (self->direction) {
  case LEFT:
    timeout(TIMEOUT);
    --self->x;
    break;
  case RIGHT:
    timeout(TIMEOUT);
    ++self->x;
    break;
  case UP:
    timeout(TIMEOUT * 1.8);
    --self->y;
    break;
  case DOWN:
    timeout(TIMEOUT * 1.8);
    ++self->y;
    break;
  }

  // allows snake to wrap around when screen edge is reached
  if (self->x >= maxx) {
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

inline static void snake_highlight_head(Snake *const self) {
  attron(RED_FG);
  mvprintw(self->y, self->x, "█");
  attroff(RED_FG);
}

void snake_draw(Snake *const self, int key, int maxy, int maxx) {
  snake_get_direction(self, key);
  snake_update(self, maxy, maxx);

  // head
  mvprintw(self->y, self->x, "█");

  // body
  if (self->player_no == PLAYER_ONE) {
    attron(GREEN_FG);
  } else if (self->player_no == PLAYER_TWO) {
    attron(YELLOW_FG);
  }
  for (size_t i = 0; i < vec_len(self->body_coords); i++) {
    Coordinate coord = {0};
    vec_get(self->body_coords, i, &coord);
    mvprintw(coord.y, coord.x, "█");
  }

  if (!self->is_alive)
    snake_highlight_head(self);

  if (self->player_no == PLAYER_ONE) {
    attroff(GREEN_FG);
  } else if (self->player_no == PLAYER_TWO) {
    attroff(YELLOW_FG);
  }
}

void snake_eat(Snake *self, Food *const food) {
  self->x = food->x;
  self->y = food->y;
  Coordinate before_head;
  // reinserts the tail to make snake body increase
  vec_get(self->body_coords, vec_len(self->body_coords) - 1, &before_head);
  vec_push(self->body_coords, &before_head);
}

void snake_check_collision(Snake *const snake1, Snake *const snake2) {
  Coordinate part;

  if (snake1->x == snake2->x && snake1->y == snake2->y) {
    snake_highlight_head(snake1);
    snake_highlight_head(snake2);
    snake1->is_alive = false;
    snake2->is_alive = false;
  }

  for (size_t i = 0; i < vec_len(snake2->body_coords); i++) {
    vec_get(snake2->body_coords, i, &part);
    if (snake1->x == part.x && snake1->y == part.y) {
      snake_highlight_head(snake1);
      snake1->is_alive = false;
    }
  }

  for (size_t i = 0; i < vec_len(snake1->body_coords); i++) {
    vec_get(snake1->body_coords, i, &part);
    if (snake2->x == part.x && snake2->y == part.y) {
      snake_highlight_head(snake2);
      snake2->is_alive = false;
    }
  }
}

Food food_new(Snake *const snake, int maxy, int maxx) {
  Food food;
  srand(time(NULL));

  for (;;) {
    food.x = rand() % maxx;
    food.y = rand() % maxy;

    // found a food position in a coordinate where the snake is not occupying
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

void score_draw(int no_players, int score1, int score2, int maxy, int maxx) {
  move(maxy, 0);
  for (int i = 0; i < maxx; i++) {
    printw("█");
  }

  attron(A_REVERSE | A_BOLD);
  char msg[100] = {0};
  switch (no_players) {
  case 1:
    snprintf(msg, sizeof(msg), "Score %d    Best: %d", score1, 0);
    break;
  case 2:
    snprintf(msg, sizeof(msg), "Score P1: %d    Score P2: %d    Best: %d",
             score1, score2, 0);
    break;
  }
  mvprintw(maxy, (maxx - strlen(msg)) / 2, "%s", msg);
  attroff(A_REVERSE | A_BOLD);
}
