#include <array>
#include <cstddef>
#include <vector>

constexpr int TIMEOUT = 80;

enum class SnakeDirection
{
  UP,
  DOWN,
  RIGHT,
  LEFT
};

class Snake
{
  SnakeDirection direction;
  size_t len;

  void get_direction();
  void check_life_status();

public:
  // head position
  int x;
  int y;

  int maxx;
  int maxy;
  bool is_alive;
  std::vector<std::array<int, 2>> body;

  Snake();
  void draw();
  void update();
  void grow();
};
