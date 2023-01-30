/*
 * When ncurses gets a SIGWINCH, it
 * sends a KEY_RESIZE char which can
 * be read using wgetch().
 */
#include <ncurses.h>
#include "maze.h"
#include "maze.h"
#include "macros.h"

struct position
{
	int i, j;
};

position player;
position start;
position finish;


Maze *maze;




void moveWin(WINDOW *win, int y, int x)
{
	wbkgd(win, COLOR_PAIR(-1));
	wrefresh(win);
	mvwin(win, y, x);
	wbkgd(win, COLOR_PAIR(3));
	wrefresh(win);
}




/*
 * Draw a window that takes
 * up the whole terminal.
 */
WINDOW* displayReady()
{
	curs_set(0);
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	int height, width, start_y, start_x;
	height = yMax;
	width = xMax;
	start_y = start_x = 0;
	WINDOW *win = newwin(height, width, start_y, start_x);
	refresh();
	box(win, 0, 0);
	return win;
}

void ready()
{
	// display the ready screen
	WINDOW *win = displayReady();
	int startRound = 0;
	while (!startRound) {
		switch (wgetch(win)) {
			case KEY_RESIZE:
				wresize(win, LINES, COLS);
				wrefresh(win);
				wclear(stdscr);
				wclear(win);
				box(win, 0, 0);
				wrefresh(win);
				break;
			case 10:
				startRound = 1;
				break;
			default:
				break;
		}
	}
	delwin(win);
	wclear(stdscr);
	wrefresh(stdscr);
}


void displayRound()
{
	//Maze m(10, 10);
	std::vector<std::vector<char>> dm = maze->getDisplayMaze();

	curs_set(0);

	refresh();


	refresh();

	for (int j = 0; j < dm[0].size(); j++)
	{
		for (int i = 0; i < dm.size(); i++)
		{
			int xstart = 2*i;
			int ystart = j;
			WINDOW *win = newwin(1, 2, ystart, xstart);
			if (dm[i][j] == '@')
				wbkgd(win, COLOR_PAIR(2));
			else
				wbkgd(win, COLOR_PAIR(-1));
			wrefresh(win);
		}
	}
	auto [i, j] = maze->getFinish();
	WINDOW *fin = newwin(1, 2, 2*j + 1, 4*i + 2);
	wbkgd(fin, COLOR_PAIR(1));
	wrefresh(fin);
}

void playRound()
{
	auto [i, j] = maze->getStart();
	/* Using random start location causes bugs
	player.i = i;
	player.j = j;
	*/
	player.i = 0;
	player.j = 0;

	WINDOW *playerWin = newwin(1, 2, 2*player.j + 1, 4*player.i + 2);
	wbkgd(playerWin, COLOR_PAIR(3));
	wrefresh(playerWin);

	int atFinish = 0;
	while (!atFinish) {
		int direction = 0;
		switch (wgetch(playerWin)) {
			case 'h':
				direction = LEFT;
				break;
			case 'j':
				direction = DOWN;
				break;
			case 'k':
				direction = UP;
				break;
			case 'l':
				direction = RIGHT;
				break;
			case KEY_RESIZE:
				direction = 0;
				break;
			default:
				direction = 0;
				break;
		}

		positionInfo newPosition;
		if (direction)
			newPosition = maze->requestMove((player.i - 2)/4, (player.j - 1)/2, direction);

		if (newPosition.isValid) {
			player.i = 4 * newPosition.i + 2;
			player.j = 2 * newPosition.j + 1;
			if (newPosition.isFinished)
				return;
		}



		moveWin(playerWin, player.j, player.i);
	}
}









int main(int argc, char **argv)
{
	initscr();
	noecho();

	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);

	for (int round = 1; round <= 10; round++)
	{


		maze = new Maze(10, 10);
		ready();



		// play round
		displayRound();
		playRound();
		delete maze;
		getch();
	}

	endwin();
	return 0;
}
