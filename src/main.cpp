#include <ncurses.h>
#include <random>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <array>
#include <unordered_set>

class Bar
{
    int y; // starting point
    int x; // length of the bar
    int maxx;
    int maxy;

public:
    Bar()
    {
        int maxy, maxx;
        getmaxyx(stdscr, maxy, maxx);
        this->y = maxy - 3;
        this->x = maxx;
        this->maxx = maxx;
        this->maxy = maxy;
    }

    void draw(int score)
    {
        for (int x = 0; x < this->x; x++)
        {
            mvprintw(this->y, x, "▔");
        }

        mvprintw((this->y + this->maxy) / 2, this->x / 2, "Score: ");
        printw("%d", score);
    }

    void show_endscreen(int score)
    {
        int midy = (this->y + this->maxy) / 2;
        for (int x = 0; x < this->x; x++)
        {
            mvprintw(midy, x, " ");
        }
        mvprintw((this->y + this->maxy) / 2, this->x * 0.4, "GAME OVER   Final Score: %d ", score);
    }
};

class Food
{
    int x;
    int y;

public:
    Food(int snake_y, int snake_x)
    {
        int maxx, maxy;
        getmaxyx(stdscr, maxy, maxx);

        bool found_position = false;
        while (!found_position)
        {
            std::srand(time(NULL));
            this->x = std::rand() % (maxx - 8);
            this->y = std::rand() % (maxy - 16);
            if (this->x != snake_x && this->y != snake_y)
            {
                found_position = true;
            }
        }

        mvprintw(this->y, this->x, "⬤");
    }

    bool is_eaten(int snake_y, int snake_x)
    {
        if (this->x == snake_x && this->y == snake_y)
        {
            return true;
        }

        return false;
    }
};

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
    std::vector<std::array<int, 2>> body;
    size_t len;

    void get_direction()
    {

        int input = getch();
        switch (input)
        {
        // arrow up
        case 'w':
            if (this->direction == SnakeDirection::DOWN)
            {
                this->draw();
                return;
            }
            this->direction = SnakeDirection::UP;
            break;
        // arrow down
        case 's':
            if (this->direction == SnakeDirection::UP)
            {
                this->draw();
                return;
            }
            this->direction = SnakeDirection::DOWN;
            break;
        // arrow right
        case 'd':
            if (this->direction == SnakeDirection::LEFT)
            {
                this->draw();
                return;
            }
            this->direction = SnakeDirection::RIGHT;
            break;
        case 'a':
            if (this->direction == SnakeDirection::RIGHT)
            {
                this->draw();
                return;
            }
            this->direction = SnakeDirection::LEFT;
            break;
        }
    }

    void check_life_status()
    {
        if ((this->x > 0 && this->x < this->maxx) && (this->y > 0 && this->y < this->maxy))
        {
            this->is_alive = true;
        }
        else
        {
            this->is_alive = false;
        }
    }

public:
    // head position
    int x;
    int y;
    int maxx;
    int maxy;
    bool is_alive;

    Snake()
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

    void draw()
    {
        this->body.push_back({this->y, this->x});
        if (this->body.size() > this->len)
        {
            auto prev = this->body.at(0);
            mvprintw(prev[0], prev[1], " ");
            this->body.erase(this->body.begin());
        }
        mvwprintw(stdscr, this->y, this->x, "█");
    }

    void update()
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
            timeout(TIMEOUT * 2);
            this->y--;
            break;
        case SnakeDirection::DOWN:
            timeout(TIMEOUT * 2);
            this->y++;
            break;
        }

        this->draw();
    }

    void grow()
    {
        this->len++;
    }
};

int main()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    timeout(TIMEOUT);
    curs_set(false);

    int score = 0;
    Bar bar{};
    bar.draw(score);

    Snake snake{};
    Food food(snake.y, snake.x);

    for (;;)
    {
        snake.update();
        if (food.is_eaten(snake.y, snake.x))
        {
            score++;
            bar.draw(score);
            food = Food(snake.y, snake.x);
            snake.grow();
            snake.grow();
        }

        if (!snake.is_alive)
        {
            bar.show_endscreen(score);
        }

        refresh();
    }
    return 0;
}
