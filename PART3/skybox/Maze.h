#ifndef MAZE_H
#define MAZE_H
#include "Brick.h"
#include <random>
#include <algorithm>
using namespace std;

// https://en.wikipedia.org/wiki/Maze_generation_algorithm

struct cell {
	int visited = -1;	// 200 if visited
	int up = 1;			// 0 if wall broken down
	int down = 1;
	int left = 1;
	int right = 1;
};

cell maze1[10][10];
int unvisited = 100;
int cc = 0;



class Maze
{
public:
	Maze() {
		cout << "maze started" << endl;
	};
	~Maze() {};



	void generate_maze(int r, int c, int up, int down, int left, int right) {

		// visited
		maze1[r][c].visited = cc++;
		unvisited -= 1;
		if (up == 1) maze1[r][c].up = 0;
		if (down == 1) maze1[r][c].down = 0;
		if (left == 1) maze1[r][c].left = 0;
		if (right == 1) maze1[r][c].right = 0;

		// info
		// cout << "cell" << r << c << "up" << maze1[r][c].up << "down" << maze1[r][c].down << "left" << maze1[r][c].left << "right" << maze1[r][c].right << endl;

		// random possible moves   { up, down, left, right }
		int moves[4] = { 0,1,2,3 };

		srand( unvisited*time(NULL) );
		random_shuffle(begin(moves), end(moves));
		//cout << moves[0] << moves[1] << moves[2] << moves[3] << endl;

		for (int i = 0; i <= 3; i++){
			if (moves[i]==0){			//up
				if (r - 1 >= 0 && maze1[r - 1][c].visited == -1) {
					maze1[r][c].up = 0;
					generate_maze(r - 1, c, 0,1,0,0);
				}
			}
			if (moves[i]==1) {			//down
				if (r + 1 <= 9 && maze1[r + 1][c].visited == -1) {
					maze1[r][c].down = 0;
					generate_maze(r + 1, c, 1,0,0,0);
				}
			}
			if (moves[i] == 2) {		//left
				if (c - 1 >= 0 && maze1[r][c - 1].visited == -1) {
					maze1[r][c].left = 0;
					generate_maze(r, c - 1, 0,0,0,1);
				}
			}
			if (moves[i]==3) {			//right
				if (c + 1 <= 9 && maze1[r][c + 1].visited == -1) {
					maze1[r][c].right = 0;
					generate_maze(r, c + 1, 0,0,1,0);
				}
			}
		}

		return;


	}

	void draw() {
		generate_maze(0,0, 0,0,0,0);


	}

	void print_maze() {
		for (int r = 0; r < 10; r++){
			for (int c = 0; c < 10; c++){
				cout << maze1[r][c].visited << " ";
			}
			cout << endl;
		}
	}


};
#endif