#ifndef GAME_H
#define GAME_H

#include "leaderboard.h"
#include "vector.h"

typedef enum {
  PLAYER_NONE,
  PLAYER_ONE,
  PLAYER_TWO,
} Player;

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN,
} Direction;

typedef struct {
  int x, y;
} Coordinate;

typedef struct {
  // stores the coordinates of the body parts on screen
  vec_Vector *body_coords;
  int x, y;
  bool is_alive;
  int player_no;
  Direction direction;
} Snake;

typedef struct {
  int x, y;
} Food;

Snake snake_new(int maxy, int maxx, Player player_no);
void snake_free(Snake *self);
void snake_draw(Snake *self, int key, int maxy, int maxx);
void snake_eat(Snake *self, Food *const food);

Food food_new(Snake *snake, int maxy, int maxx);
void food_draw(Food *self);
bool food_is_eaten(Food *self, Snake *const snake);

void score_draw(int no_players, int score1, int score2, int top_score, int maxy,
                int maxx);
void snake_check_collision(Snake *const snake1, Snake *const snake2);

#endif
