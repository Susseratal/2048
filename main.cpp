#include <iostream>
#include <string>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define max_digits 8

using namespace std;

typedef int Grid[4][4];

enum direction { d_left, d_down, d_up, d_right };

void addNumber(Grid& grid)
{
    int randX, randY;
    do
    {
        randX = rand() % 4;
        randY = rand() % 4;
    } while (grid[randY][randX] != 0);

    grid[randY][randX] = 2;
}

void drawTable(WINDOW* win)
{
    for (int y = 5; y < 22; y++)
    {
        mvwaddstr(win, y, 5, "|         |         |         |         |");
    }

    for (int y = 5; y < 22; y+=4)
    {
        mvwaddstr(win, y, 5, "|- - - - -|- - - - -|- - - - -|- - - - -|");
    }
}

void drawGrid(Grid& grid, WINDOW* win)
{
    drawTable(win);

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (grid[y][x] == 0)
            {
                // don't draw anything
            }
            else
            {
                char tmp[max_digits];
                snprintf(tmp, max_digits, "%d", grid[y][x]);
                mvwaddstr(win, 7 + (y * 4), 10 + (x * 10), tmp);
            }
        }
    }
    wrefresh(win);
}

bool move(Grid& grid, direction d, WINDOW* win)
{
    bool changed = false;
    switch (d)
    {
        case d_left:
            for (int y = 0; y < 4; y++)
            {
                for (int x = 1; x < 4; x++)
                {
                    for (int i = x - 1; i >= 0; i--)
                    {
                        if(grid[y][i] == grid[y][x])
                        {
                            if (grid[y][i] != 0)
                            {
                                grid[y][i] *= 2;
                                grid[y][x] = 0;
                                changed = true;
                            }
                            break;
                        }

                        else if(grid[y][i] != 0)
                        {
                            if (i + 1 != x)
                            {
                                int tmp = grid[y][x];
                                grid[y][x] = 0;
                                grid[y][i + 1] = tmp;
                                changed = true;
                            }
                            break;
                        }

                        else if(i == 0 && grid[y][i] == 0 && grid[y][x] != 0)
                        {
                            grid[y][i] = grid[y][x];
                            grid[y][x] = 0;
                            changed = true;
                            break;
                        }
                    }
                }
            }
            break;

        case d_down:
            for (int x = 0; x < 4; x++)
            {
                for (int y = 2; y >= 0; y--)
                {
                    for (int i = y + 1; i < 4; i++)
                    {
                        if(grid[i][x] == grid[y][x])
                        {
                            if (grid[i][x] != 0)
                            {
                                grid[i][x] *= 2;
                                grid[y][x] = 0;
                                changed = true;
                            }
                            break;
                        }
                        else if(grid[i][x] != 0)
                        {
                            if (i - 1 != y)
                            {
                                int tmp = grid[y][x];
                                grid[y][x] = 0;
                                grid[i - 1][x] = tmp;
                                changed = true;
                            }
                            break;
                        }
                        else if(i == 3 && grid[i][x] == 0)
                        {
                            grid[i][x] = grid[y][x];
                            grid[y][x] = 0;
                            changed = true;
                            break;
                        }
                    }
                }
            }
            break;

        case d_up:
            for (int x = 0; x < 4; x++)
            {
                for (int y = 1; y < 4; y++)
                {
                    for (int i = y - 1; i >= 0; i--)
                    {
                        if(grid[i][x] == grid[y][x])
                        {
                            if (grid[i][x] != 0)
                            {
                                grid[i][x] *= 2;
                                grid[y][x] = 0;
                                changed = true;
                            }
                            break;
                        }
                        else if(grid[i][x] != 0)
                        {
                            if (i + 1 != y)
                            {
                                int tmp = grid[y][x];
                                grid[y][x] = 0;
                                grid[i + 1][x] = tmp;
                                changed = true;
                            }
                            break;
                        }
                        else if(i == 0 && grid[i][x] == 0)
                        {
                            grid[i][x] = grid[y][x];
                            grid[y][x] = 0;
                            changed = true;
                            break;
                        }
                    }
                }
            }
            break;

        case d_right:
            for (int y = 0; y < 4; y++)
            {
                for (int x = 2; x >= 0; x--)
                {
                    for (int i = x + 1; i < 4; i++)
                    {
                        if(grid[y][i] == grid[y][x])
                        {
                            if (grid[y][i] != 0)
                            {
                                grid[y][i] *= 2;
                                grid[y][x] = 0;
                                changed = true;
                            }
                            break;
                        }

                        else if(grid[y][i] != 0)
                        {
                            if (i - 1 != x)
                            {
                                int tmp = grid[y][x];
                                grid[y][x] = 0;
                                grid[y][i - 1] = tmp;
                                changed = true;
                            }
                            break;
                        }

                        else if(i == 3 && grid[y][i] == 0)
                        {
                            grid[y][i] = grid[y][x];
                            grid[y][x] = 0;
                            changed = true;
                            break;
                        }
                    }

                }
            }
            break;
    }
    return changed;
}

int main()
{
    // Initialise ncurses screen 
    initscr();

    // create ncurses subwindow to fill screen
    int h, w;
    getmaxyx(stdscr, h, w);
    WINDOW* win = newwin(h, w, 0, 0);

    // set keyboard settings
    keypad(win, TRUE);
    noecho();
    curs_set(0);

    // initialise a playing grid
    Grid playingGrid;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            playingGrid[i][n] = 0;
        }
    }

    // generate random numbers for populating grid
    srand(time(NULL));

    // populate the playing grid with a couple of numbers
    for (int i = 0; i < 2; i++)
    {
        addNumber(playingGrid);
    }

    drawGrid(playingGrid, win);


    // core gameplay loop
    while (1)
    {
        bool changed = false;
        int ch = wgetch(win);
        switch (ch)
        {
            case KEY_LEFT:
                wclear(win);
                changed = move(playingGrid, d_left, win);
                drawGrid(playingGrid, win);
                break;

            case KEY_DOWN:
                wclear(win);
                changed = move(playingGrid, d_down, win);
                drawGrid(playingGrid, win);
                break;

            case KEY_UP:
                wclear(win);
                changed = move(playingGrid, d_up, win);
                drawGrid(playingGrid, win);
                break; 

            case KEY_RIGHT:
                wclear(win);
                changed = move(playingGrid, d_right, win);
                drawGrid(playingGrid, win);
                break; 

            case 'q':
                endwin();
                return 0;
                break;

            default:
                continue;
        }

        if (changed)
        {
            addNumber(playingGrid);
        }


        /*
        // check if the grid is full
        for (int y = 0; y < 4; y++)
        {
            // this is where it has to look at everything in the grid 
            // if it has a value, the counter goes up
            // if the grid is full, game over
        }
        */
    }
}
