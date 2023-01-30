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
}




std::vector<int> Maze::ij(int index)
{
	std::vector<int> ij;
	int i, j;
	j = floor((double)index / (double)w);
	i = index % w;
	ij.push_back(i);
	ij.push_back(j);
	return ij;
}



int Maze::indx(int i, int j)
{
	return j*w + i;
}




void Maze::createMaze(int w, int h)
{
	srand(time(NULL));

	/* Populate the grid with initial values of zero. */
	for (int i = 0; i < w*h; i++)
		maze.push_back(0x0);



	/* Create the maze. */
	std::stack<int> stk;
	stk.push(0);
	std::unordered_set<int> visited;
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


std::vector<int> Maze::getMaze()
{
	return maze;
}



std::vector<int> Maze::getAvailableNeighbours(int index, std::unordered_set<int> visited)
{
	std::vector<int> coords = ij(index);
	int i, j;
	i = coords[0];
	j = coords[1];

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
	for (int I = 0; I < W*H; I++)
		displayMaze.push_back(wall);

	for (int i = 0; i < w; i++)
	{
		int I = 2*i + 1;
		for (int j = 0; j < h; j++)
		{
			int J = 2*j + 1;
			displayMaze[J*W + I] = route;

			if (maze[indx(i, j)] & LEFT)
				displayMaze[J*W + I - 1] = route;

			if (maze[indx(i, j)] & RIGHT)
				displayMaze[J*W + I + 1] = route;

			if (maze[indx(i, j)] & UP)
				displayMaze[J*W + I - W] = route;

			if (maze[indx(i, j)] & DOWN)
				displayMaze[J*W + I + W] = route;
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


std::vector<std::vector<int>> Maze::getAdj()
{
	return adj;
}








void Maze::print()
{
	for (int J = 0; J < H; J++)
	{
		for (int I = 0; I < W; I++)
		{
			std::cout << displayMaze[J*W + I] << " ";
		}
		std::cout << std::endl;
	}
}



void Maze::printM()
{
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			std::cout << maze[indx(i, j)] << " ";
		}
		std::cout << std::endl;
	}
}




void Maze::printAdj()
{
	for (int i = 0; i < adj.size(); i++)
	{
		std::cout << "Node " << i << ": ";
		for (int j = 0; j < adj[i].size(); j++)
			std::cout << adj[i][j] << " ";
		std::cout << std::endl;
	}
}




/*
 * Find shortest path between source and target.
 *
 * It is slow because it uses a linear search
 * when looking for the node not in 'notVisited'
 * with the smallest distance.
 * We could instead use a priority queue.
 */
void Maze::slowDijkstra()
{
	std::set<int> notVisited;
	std::vector<int> dist;
	std::vector<int> prev;
	int source = 0;
	int target = w*h - 1;

	for (int index = 0; index < maze.size(); index++)
	{
		notVisited.insert(index);
		dist.push_back(9999);
		prev.push_back(-1);
	}

	dist[source] = 0;


	int currentNode;
	int found = 0;

	while (!notVisited.empty() && !found)
	{

		/*
		 * Get node in notVisited with smallest value
		 * and remove it from notVisited.
		 */
		int minDist = 9999;
		int currentNode = 0;
		for (auto it = notVisited.begin(); it != notVisited.end(); it++)
		{
			if (dist[*it] < minDist)
			{
				minDist = dist[*it];
				currentNode = *it;
			}
		}
		notVisited.erase(currentNode);

		/*
		 * Get node in notVisited with smallest value
		 * and remove it from notVisited.
		 */
		for (int n = 0; n < adj[currentNode].size(); n++)
		{
			int neighbour = adj[currentNode][n];
			if (notVisited.count(neighbour))
			{
				if (dist[neighbour] > dist[currentNode] + 1)
				{
					dist[neighbour] = dist[currentNode] + 1;
					prev[neighbour] = currentNode;
					if (neighbour == target)
						found = 1;
				}
			}
		}
	}

	/*
	 * Backtrack to print route.
	 * Change this to fill in the path on displayMaze.
	 */
	std::map<int, int> trail;
	currentNode = target;
	while (currentNode != source)
	{
		trail[prev[currentNode]] = currentNode;
		currentNode = prev[currentNode];
	}

	printPath(trail, source, target);
}



/*
 * Prints the 'displayMaze' with the path taken.
 */
void Maze::printPath(std::map<int, int> trail, int source, int target)
{

	// get positions of minus and pipe characters
	std::map<int, char> transitions;
	for (auto it = trail.begin(); it != trail.end(); it++)
	{
		int I;
		int J;
		I = (ij(it->first)[0])*2 + 1;
		J = (ij(it->first)[1])*2 + 1;

		if (it->second == it->first - 1)      // gone left
		{
			transitions[J*W + I - 1] = '-';
		}
		else if (it->second == it->first + 1) // gone right
		{
			transitions[J*W + I + 1] = '-';
		}
		else if (it->second == it->first - w) // gone up
		{
			transitions[J*W + I - W] = '|';
		}
		else if (it->second == it->first + w) // gone down
		{
			transitions[J*W + I + W] = '|';
		}
	}




	// alter the maze
	std::vector<char> mazeTmp = displayMaze;
	for (int J = 0; J < H; J++)
	{
		for (int I = 0; I < W; I++)
		{
			if (J % 2 != 0 && I % 2 != 0) // if we are not on a wall
			{
				int i = (I - 1)/2;
				int j = (J - 1)/2;
				if (trail.count(indx(i, j)))
				{
					mazeTmp[J*W + I] = '+';
				}
				else if (indx(i, j) == target)
				{
					mazeTmp[J*W + I] = '+';
				}
			}

			else if (transitions.count(J*W + I))
			{
				mazeTmp[J*W + I] = transitions[J*W + I];
			}
		}
	}



	// print the maze
	for (int J = 0; J < H; J++)
	{
		for (int I = 0; I < W; I++)
		{
			std::cout << mazeTmp[J*W + I] << " ";
		}
		std::cout << std::endl;
	}
}
