#include <vector>
#include <iostream>
#include <ncurses.h>

#include "maze.h"
#include "macros.h"



void initMaze()
{
	Maze m(5, 5);
	std::vector<std::vector<char>> dm = m.getDisplayMaze();
	positionInfo newPos = m.requestMove(0, 0, RIGHT);
}



void displayTheMaze()
{
	Maze m(5, 5);
	std::vector<std::vector<char>> dm = m.getDisplayMaze();

	for (int j = 0; j < dm[0].size(); j++)
	{
		for (int i = 0; i < dm.size(); i++)
		{
			std::cout << dm[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

void frontendTest()
{
	Maze m(9, 9);
	std::vector<std::vector<char>> dm = m.getDisplayMaze();

	initscr();
	curs_set(0);
	refresh();

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);

	bkgd(COLOR_PAIR(1));
	refresh();

	for (int j = 0; j < dm[0].size(); j++)
	{
		for (int i = 0; i < dm.size(); i++)
		{
			int xstart = 2*i + 1;
			int ystart = j;
			WINDOW *win = newwin(1, 2, ystart, xstart);
			if (dm[i][j] == '@')
				wbkgd(win, COLOR_PAIR(2));
			else
				wbkgd(win, COLOR_PAIR(1));
			wrefresh(win);
		}
	}

	getch();
	endwin();
}






int main()
{
	initMaze();
	displayTheMaze();
	frontendTest();
	return 0;
}
