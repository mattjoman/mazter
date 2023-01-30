#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <math.h>

#include "maze.h"
#include "macros.h"


void err(const char* message)
{
	std::cout << message << std::endl;
	exit(1);
}



int randomInt(int from, int to)
{
	return from + (rand() % (to - from + 1));
}



Maze::Maze(int w, int h) : w{w}, h{h}
{
	W = 2*w + 1;
	H = 2*h + 1;
	createMaze(w, h);
	createAdj();
	createDisplayMaze();
	startIndex  = randomInt(0, w*h - 1);
	finishIndex = randomInt(0, h*h - 1);
}




std::pair<int, int> Maze::ij(int index)
{
	int i, j;
	j = floor((double)index / (double)w);
	i = index % w;
	return {i, j};
}



int Maze::indx(int i, int j)
{
	return j*w + i;
}











/* -------------------------------Maze Generation------------------------------ */

void Maze::createMaze(int w, int h)
{
	srand(time(NULL));

	/* Populate the grid with initial values of zero. */
	for (int i = 0; i < w*h; i++)
		maze.push_back(0x0);



	/* Create the maze. */
	std::stack<int> stk;
	std::unordered_set<int> visited;

	stk.push(0);
	visited.insert(0);

	int complete = 0;
	int current;

	while (!complete)
	{

		current = stk.top();
		std::vector<int> neighbours = getAvailableNeighbours(current, visited);

		if (neighbours.size() > 0)
		{
			// make random choice
			int rnum = randomInt(0, neighbours.size()-1);
			int next = neighbours[rnum];

			// set bits for current and next
			if (next == current + w) // gone down
			{
				maze[current] |= DOWN;
				maze[next]    |= UP;
			}
			else if (next == current - w) // gone up
			{
				maze[current] |= UP;
				maze[next]    |= DOWN;
			}
			else if (next == current + 1) // gone right
			{
				maze[current] |= RIGHT;
				maze[next]    |= LEFT;
			}
			else if (next == current - 1) // gone left
			{
				maze[current] |= LEFT;
				maze[next]    |= RIGHT;
			}
			else
			{
				err("Next index makes no sense!");
			}

			stk.push(next);
			visited.insert(next);
		}
		else if (neighbours.size() == 0)
		{
			stk.pop();
		}
		else
		{
			err("problem with neighbours vector");
		}

		neighbours.clear();

		if (stk.size() == 0 && visited.size() == w*h)
			complete = 1;
	}
}






std::vector<int> Maze::getAvailableNeighbours(int index, std::unordered_set<int> visited)
{
	auto [i, j] = ij(index);

	int neighIndex;
	std::vector<int> neighboursTmp;

	/* Add neighbours if not out-of-bounds. */
	if (i != 0)
	{
		neighIndex = indx(i-1, j);
		neighboursTmp.push_back(neighIndex);
	}
	if (i != (w - 1))
	{
		neighIndex = indx(i+1, j);
		neighboursTmp.push_back(neighIndex);
	}
	if (j != 0)
	{
		neighIndex = indx(i, j-1);
		neighboursTmp.push_back(neighIndex);
	}
	if (j != (h - 1))
	{
		neighIndex = indx(i, j+1);
		neighboursTmp.push_back(neighIndex);
	}

	/* Remove neighbours that are already visited. */
	std::vector<int> neighbours;
	for (int k = 0; k < neighboursTmp.size(); k++)
	{
		if (visited.find(neighboursTmp[k]) == visited.end())
		{
			neighbours.push_back(neighboursTmp[k]);
		}
	}
	return neighbours;
}





void Maze::createDisplayMaze()
{
	/* Populate the display grid with initial values of 'M'. */
	char wall  = '@';
	char route = ' ';
	for (int I = 0; I < W; I++)
	{
		std::vector<char> vj;
		for (int J = 0; J < H; J++)
		{
			vj.push_back(wall);
		}
		displayMaze.push_back(vj);
	}

	for (int i = 0; i < w; i++)
	{
		int I = 2*i + 1;
		for (int j = 0; j < h; j++)
		{
			int J = 2*j + 1;

			displayMaze[I][J] = route;

			if (maze[indx(i, j)] & LEFT)
				displayMaze[I-1][J] = route;

			if (maze[indx(i, j)] & RIGHT)
				displayMaze[I+1][J] = route;

			if (maze[indx(i, j)] & UP)
				displayMaze[I][J-1] = route;

			if (maze[indx(i, j)] & DOWN)
				displayMaze[I][J+1] = route;
		}
	}
}




void Maze::createAdj()
{
	for (int index = 0; index < w*h; index++)
	{
		std::vector<int> node;
		if (maze[index] & LEFT)
			node.push_back(index - 1);
		if (maze[index] & RIGHT)
			node.push_back(index + 1);
		if (maze[index] & UP)
			node.push_back(index - w);
		if (maze[index] & DOWN)
			node.push_back(index + w);
		adj.push_back(node);
	}
}


































/* Getter function for displayMaze. */
std::vector<std::vector<char>> Maze::getDisplayMaze()
{
	return displayMaze;
}




/*
 * Given a position and direction
 * return 1 if the direction is
 * valid, else return 0.
 */
int Maze::isValidDirection(int i, int j, int direction)
{
	int isValid = 0;
	int index = indx(i, j);
	if (maze[index] & direction)
		isValid = 1;
	return isValid;
}



/*
 * Given a position and direction,
 * return info about the position.
 */
positionInfo Maze::requestMove(int i, int j, int direction)
{
	positionInfo newPos;

	// if direction is not valid
	if (!isValidDirection(i, j, direction)) {
		newPos.isValid = 0;
		return newPos;
	}

	// get new i, j
	newPos.isValid = 1;
	newPos.i = i;
	newPos.j = j;
	switch (direction) {
		case LEFT:
			newPos.i--;
			break;
		case RIGHT:
			newPos.i++;
			break;
		case UP:
			newPos.j--;
			break;
		case DOWN:
			newPos.j++;
			break;
		default:
			break; // error instead
	}

	newPos.isFinished = 0;
	if (isAtFinish(newPos.i, newPos.j))
		newPos.isFinished = 1;

	return newPos;
}


int Maze::isAtFinish(int i, int j)
{
	if (indx(i, j) == finishIndex)
		return 1;

	return 0;
}


std::pair<int, int> Maze::getStart()
{
	return ij(startIndex);
}

std::pair<int, int> Maze::getFinish()
{
	return ij(finishIndex);
}







