#include <iostream>
#include <string>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define max_digits 8

using namespace std;

typedef int Grid[4][4];

enum direction { d_left, d_down, d_up, d_right };

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

void move(Grid& grid, direction d, WINDOW* win)
{
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
                            grid[y][i] *= 2;
                            grid[y][x] = 0;
                            break;
                        }

                        else if(grid[y][i] != 0)
                        {
                            int tmp = grid[y][x];
                            grid[y][x] = 0;
                            grid[y][i + 1] = tmp;
                            break;
                        }

                        else if(i == 0 && grid[y][i] == 0)
                        {
                            grid[y][i] = grid[y][x];
                            grid[y][x] = 0;
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
                            grid[i][x] *= 2;
                            grid[y][x] = 0;
                            break;
                        }
                        else if(grid[i][x] != 0)
                        {
                            int tmp = grid[y][x];
                            grid[y][x] = 0;
                            grid[i - 1][x] = tmp;
                            break;
                        }
                        else if(i == 3 && grid[i][x] == 0)
                        {
                            grid[i][x] = grid[y][x];
                            grid[y][x] = 0;
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
                            grid[i][x] *= 2;
                            grid[y][x] = 0;
                            break;
                        }
                        else if(grid[i][x] != 0)
                        {
                            int tmp = grid[y][x];
                            grid[y][x] = 0;
                            grid[i + 1][x] = tmp;
                            break;
                        }
                        else if(i == 0 && grid[i][x] == 0)
                        {
                            grid[i][x] = grid[y][x];
                            grid[y][x] = 0;
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
                            grid[y][i] *= 2;
                            grid[y][x] = 0;
                            break;
                        }

                        else if(grid[y][i] != 0)
                        {
                            int tmp = grid[y][x];
                            grid[y][x] = 0;
                            grid[y][i - 1] = tmp;
                            break;
                        }

                        else if(i == 3 && grid[y][i] == 0)
                        {
                            grid[y][i] = grid[y][x];
                            grid[y][x] = 0;
                            break;
                        }
                    }

                }
            }
            break;
    }
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
    int randX, randY;

    // populate the playing grid with a couple of numbers
    for (int i = 0; i < 2; i++)
    {
        do
        {
            randX = rand() % 4;
            randY = rand() % 4;
        } while (playingGrid[randY][randX] != 0);

        playingGrid[randY][randX] = 2;
    }

    drawGrid(playingGrid, win);

    // core gameplay loop
    while (1)
    {
        int ch = wgetch(win);
        switch (ch)
        {
            case KEY_LEFT:
                wclear(win);
                move(playingGrid, d_left, win);
                drawGrid(playingGrid, win);
                break;

            case KEY_DOWN:
                wclear(win);
                move(playingGrid, d_down, win);
                drawGrid(playingGrid, win);
                break;

            case KEY_UP:
                wclear(win);
                move(playingGrid, d_up, win);
                drawGrid(playingGrid, win);
                break; 

            case KEY_RIGHT:
                wclear(win);
                move(playingGrid, d_right, win);
                drawGrid(playingGrid, win);
                break; 

            case 'q':
                endwin();
                return 0;
                break;
        }
    }
}
