#include <vector>
#include <iostream>

#include "maze.h"

int main()
{
	int w, h;
	std::cout << "Enter width: ";
	std::cin >> w;
	std::cout << std::endl;
	std::cout << "Enter height: ";
	std::cin >> h;
	std::cout << std::endl;
	Maze m(w, h);
	m.print();
	m.printM();
	//m.printAdj();

	//std::vector<int> maze = m.getMaze();
	
	m.slowDijkstra();

	return 0;
}
