#include <iostream>
#include <string>
#include <ncurses.h>

#define max_digits 8

using namespace std;

typedef int Grid[4][4];

enum direction { left, down, up, right };

void drawGrid(Grid& grid, WINDOW* win)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            char tmp[max_digits];
            snprintf(tmp, max_digits, "%d", grid[y][x]);
            mvwaddstr(win, y + 1, x + 1, tmp);
        }
    }
    wrefresh(win);
}

void move(Grid& grid, direction d, WINDOW* win)
{
    switch (d)
    {
        case left:
            mvwaddstr(win, 1, 1, "left");
            // move left logic
        case down:
            mvwaddstr(win, 1, 1, "down");
            // move down logic
        case up:
            mvwaddstr(win, 1, 1, "up");
            // move up logic
        case right:
            mvwaddstr(win, 1, 1, "right");
            // move right logic
    }
}

int main()
{
    Grid playingGrid;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            playingGrid[i][n] = 0;
        }
    }


    initscr();
    int h, w;
    getmaxyx(stdscr, h, w);
    WINDOW* win = newwin(h, w, 0, 0);

    keypad(win, TRUE);
    noecho();
    drawGrid(playingGrid, win);
    // start with 2 2's in random positions on the grid

    while (1)
    {
        int ch = wgetch(win);
        switch (ch)
        {
            case KEY_LEFT:
                move(playingGrid, direction::left, win);
                drawGrid(playingGrid, win);
                break;

            case KEY_DOWN:
                move(playingGrid, direction::down, win);
                drawGrid(playingGrid, win);
                break;

            case KEY_UP:
                move(playingGrid, direction::up, win);
                drawGrid(playingGrid, win);
                break; 

            case KEY_RIGHT:
                move(playingGrid, direction::right, win);
                drawGrid(playingGrid, win);
                break; 

            case 'q':
                endwin();
                return 0;
                break;
        }
    }
}
