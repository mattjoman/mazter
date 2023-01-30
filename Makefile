game: maze
	g++ -g -c game.cpp -o game.o
	g++ -g -o game game.o maze.o -lncurses

test: maze test.cpp
	g++ -g -c test.cpp -o test.o
	g++ -g -Wall -o test test.o maze.o -lncurses

maze:
	g++ -g -c maze.cpp -o maze.o

clean:
	rm *.o *test *game
