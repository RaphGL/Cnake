#include "bar.hpp"
#include <ncurses.h>
#include <string>

Bar::Bar()
{
  int maxy, maxx;
  getmaxyx(stdscr, maxy, maxx);
  this->y = maxy - 3;
  this->x = maxx;
  this->maxx = maxx;
  this->maxy = maxy;
}

void Bar::draw(int score)
{
  for (int x = 0; x < this->x; x++)
  {
    mvprintw(this->y, x, "▔");
  }

  mvprintw((this->y + this->maxy) / 2, this->x / 2, "Score: ");
  printw("%d", score);
}

void Bar::show_endscreen(int score)
{
  int midy = (this->y + this->maxy) / 2;
  for (int x = 0; x < this->x; x++)
  {
    mvprintw(midy, x, " ");
  }

  std::string msg{"Final Score: %d"};
  mvprintw((this->y + this->maxy) / 2, this->x / 2 - msg.size() / 2,
           msg.c_str(), score);

  std::string again{"GAME OVER"};
  mvprintw((this->maxy + 3) / 2, this->maxx / 2 - again.size() / 2, "%s",
           again.c_str());
}
