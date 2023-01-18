#include "snake.hpp"
#include <ncurses.h>

Snake::Snake()
{
  int maxx;
  int maxy;
  getmaxyx(stdscr, maxy, maxx);

  this->x = maxx / 2;
  this->y = maxy / 2;
  this->maxx = maxx - 1;
  this->maxy = maxy - 4;
  this->direction = SnakeDirection::LEFT;
  this->is_alive = true;
  this->len = 3;
}

void Snake::draw()
{
  this->body.push_back({this->y, this->x});
  if (this->body.size() > this->len)
  {
    auto prev = this->body.at(0);
    mvprintw(prev[0], prev[1], " ");
    this->body.erase(this->body.begin());
  }
  mvprintw(this->y, this->x, "█");
}

void Snake::get_direction()
{

  int input = getch();
  switch (input)
  {
  case 'p':
    mvprintw(this->maxy / 2, this->maxx / 2, "PAUSED");
    for (;;)
    {
      if (getch() == 'p')
      {
        mvprintw(this->maxy / 2, this->maxx / 2, "      ");
        return;
      }
    }
    break;
  case 'w':
  case KEY_UP:
    if (this->direction == SnakeDirection::DOWN)
    {
      return;
    }
    this->direction = SnakeDirection::UP;
    break;
  case 's':
  case KEY_DOWN:
    if (this->direction == SnakeDirection::UP)
    {
      return;
    }
    this->direction = SnakeDirection::DOWN;
    break;
  case 'd':
  case KEY_RIGHT:
    if (this->direction == SnakeDirection::LEFT)
    {
      return;
    }
    this->direction = SnakeDirection::RIGHT;
    break;
  case 'a':
  case KEY_LEFT:
    if (this->direction == SnakeDirection::RIGHT)
    {
      return;
    }
    this->direction = SnakeDirection::LEFT;
    break;
  }
}

void Snake::check_life_status()
{
  if (this->body.size() < 3)
  {
    return;
  }

  auto head = this->body.back();
  for (auto i = this->body.begin(); i != --this->body.end(); i++)
  {
    if (i->at(0) == head.at(0) && i->at(1) == head.at(1))
    {
      this->is_alive = false;
      return;
    }
  }
}

void Snake::update()
{
  this->get_direction();
  this->check_life_status();

  if (!is_alive)
  {
    return;
  }

  switch (this->direction)
  {
  case SnakeDirection::LEFT:
    timeout(TIMEOUT);
    this->x--;
    break;
  case SnakeDirection::RIGHT:
    timeout(TIMEOUT);
    this->x++;
    break;
  case SnakeDirection::UP:
    timeout(TIMEOUT * 1.8);
    this->y--;
    break;
  case SnakeDirection::DOWN:
    timeout(TIMEOUT * 1.8);
    this->y++;
    break;
  }

  if (this->x > this->maxx)
  {
    this->x = 0;
  }

  if (this->x < 0)
  {
    this->x = this->maxx;
  }

  if (this->y > this->maxy)
  {
    this->y = 0;
  }

  if (this->y < 0)
  {
    this->y = this->maxy;
  }

  this->draw();
}

void Snake::grow() { this->len++; }
