#include <array>
#include <vector>

class Food
{
  int x;
  int y;

public:
  Food(std::vector<std::array<int, 2>> &snake_body);
  void draw();
  // takes the coordinate of the snake's head
  bool is_eaten(int snake_y, int snake_x);
};
