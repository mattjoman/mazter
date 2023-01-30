#pragma once

#include <cstddef>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>

void err(const char* message);

int randomInt(int from, int to);



/*
 * This is the info that the Maze class
 * gives to the front end.
 */
struct positionInfo
{
	int isValid;    // is the position valid
	int i;          // position of player
	int j;          // position of player
	int isFinished; // is player at the end
};




class Maze
{
	private:
		int w;
		int h;
		int W; // blown up for the display maze
		int H; // blown up for the display maze

		int startIndex;
		int finishIndex;

		std::vector<int>               maze;
		std::vector<std::vector<int>>  adj;
		std::vector<std::vector<char>> displayMaze;

		/* -------------------------------Helper Functiions------------------------------ */
		int                 isValidDirection(int i, int j, int direction);
		int                 isAtFinish(int i, int j);
		int                 indx(int i, int j);                
		std::pair<int, int> ij(int index);

		/* -------------------------------Maze Generation------------------------------ */
		void             createMaze(int w, int h);
		std::vector<int> getAvailableNeighbours(int index, std::unordered_set<int> visited);
		void             createAdj();
		void             createDisplayMaze();

	public:
		Maze(int w, int h);
		std::vector<std::vector<char>> getDisplayMaze();
		positionInfo                   requestMove(int i, int j, int direction);
		std::pair<int, int> getStart();
		std::pair<int, int> getFinish();
};
