#include <ncurses.h>
#include <random>
#include <time.h>
#include <vector>
#include <array>
#include <csignal>

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

        std::string msg{"Final Score: %d"};
        mvprintw((this->y + this->maxy) / 2, this->x / 2 - msg.size() / 2, msg.c_str(), score);

        std::string again{"GAME OVER"};
        mvprintw((this->maxy + 3) / 2, this->maxx / 2 - again.size() / 2, again.c_str());
    }
};

class Food
{
    int x;
    int y;

public:
    Food(std::vector<std::array<int, 2>> &body)
    {
        int maxx, maxy;
        getmaxyx(stdscr, maxy, maxx);

        for (;;)
        {
            std::srand(time(NULL));
            this->x = std::rand() % (maxx - 3);
            this->y = std::rand() % (maxy - 4 - 3);

            // makes loop break if a new food position is found that doesn't overlap with the snake body
            bool found_position = true;
            for (const auto &i : body) // i[0] == snake.x && i[1] == snake.y
            {
                if (this->x == i.at(0) && this->y == i.at(1))
                {
                    found_position = false;
                    break;
                }
            }

            if (found_position && this->x > 3 && this->y > 3)
            {
                break;
            }
        }

        mvprintw(this->y, this->x, "▒");
    }

    void draw()
    {
        mvprintw(this->y, this->x, "▒");
    }

    // takes the coordinate of the snake's head
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
    size_t len;

    void get_direction()
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

    void check_life_status()
    {
        if ((this->x > 0 && this->x < this->maxx) && (this->y > 0 && this->y < this->maxy))
        {
            this->is_alive = true;

            if (this->body.size() > 3)
            {
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
    std::vector<std::array<int, 2>> body;

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
            timeout(TIMEOUT * 1.8);
            this->y--;
            break;
        case SnakeDirection::DOWN:
            timeout(TIMEOUT * 1.8);
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
    Food food(snake.body);

    signal(SIGINT, [](int signum)
           {
        endwin();
        exit(signum); });

    for (;;)
    {
        snake.update();
        food.draw();
        if (food.is_eaten(snake.y, snake.x))
        {
            score++;
            bar.draw(score);
            food = Food(snake.body);
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
