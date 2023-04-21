#include "bar.hpp"
#include <ncurses.h>
#include <string>
#include <string_view>

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

  char msg[] = "Final Score:    ";
  int centered_text = (this->x / 2 + static_cast<int>(sizeof(msg) / sizeof(char))) / 2;
  mvprintw((this->y + this->maxy) / 2, centered_text,
           "Final Score: %d", score);

  char again[] = "GAME OVER";
  mvprintw((this->maxy + 3) / 2, this->maxx / 2 - static_cast<int>(sizeof(again) / sizeof(char)) / 2, "%s",
           again);
}
