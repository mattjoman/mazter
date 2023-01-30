#include <cstddef>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>

void err(const char* message);

int randomInt(int from, int to);

class Maze
{
	private:
		int w;
		int h;
		int W; // blown up for the display maze
		int H; // blown up for the display maze
		std::vector<int> maze;
		std::vector<std::vector<int>> adj;
		std::vector<char> displayMaze;

		void createMaze(int w, int h);
		void createAdj();
		void createDisplayMaze();
		void printPath(std::map<int, int> trail, int source, int target);
		std::vector<int> getAvailableNeighbours(int index, std::unordered_set<int> visited);
		std::vector<int> ij(int index);        // go from index to i and j
		int indx(int i, int j);  // go from i and j to index

	public:
		Maze(int w, int h);
		void printM();
		void print();
		void printAdj();
		std::vector<int> getMaze();
		std::vector<std::vector<int>> getAdj();
		void slowDijkstra();
};
