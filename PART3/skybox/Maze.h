#ifndef MAZE_H
#define MAZE_H
#include "Brick.h"
#include <random>
#include <algorithm>
#include <vector>
using namespace std;

// https://en.wikipedia.org/wiki/Maze_generation_algorithm

class Maze
{
public:
	struct cell {
		int visited = -1;	// +ve if visited
		int up = 1;			// 0 if wall broken down
		int down = 1;
		int left = 1;
		int right = 1;
	};
	cell maze_path[10][10];		// maze path

	int maze_walls[21][21];		// walls for that maze
	int unvisited = 100;
	int cc = 0;

	struct wall {
		Brick* brickk;
		vec3 loc;
	};
	vector<wall> wall_list;		// list of wall bricks

	Maze() {
		cout << "maze started" << endl;
	};
	~Maze() {};



	void generate_maze_path(int r, int c, int up, int down, int left, int right) {

		// visited
		maze_path[r][c].visited = cc++;
		unvisited -= 1;
		if (up == 1) maze_path[r][c].up = 0;
		if (down == 1) maze_path[r][c].down = 0;
		if (left == 1) maze_path[r][c].left = 0;
		if (right == 1) maze_path[r][c].right = 0;

		// info
		// cout << "cell" << r << c << "up" << maze1[r][c].up << "down" << maze1[r][c].down << "left" << maze1[r][c].left << "right" << maze1[r][c].right << endl;

		// random possible moves   { up, down, left, right }
		int moves[4] = { 0,1,2,3 };

		srand( unvisited*time(NULL) );
		random_shuffle(begin(moves), end(moves));
		//cout << moves[0] << moves[1] << moves[2] << moves[3] << endl;

		for (int i = 0; i <= 3; i++){
			if (moves[i]==0){			//up
				if (r - 1 >= 0 && maze_path[r - 1][c].visited == -1) {
					maze_path[r][c].up = 0;
					generate_maze_path(r - 1, c, 0,1,0,0);
				}
			}
			if (moves[i]==1) {			//down
				if (r + 1 <= 9 && maze_path[r + 1][c].visited == -1) {
					maze_path[r][c].down = 0;
					generate_maze_path(r + 1, c, 1,0,0,0);
				}
			}
			if (moves[i] == 2) {		//left
				if (c - 1 >= 0 && maze_path[r][c - 1].visited == -1) {
					maze_path[r][c].left = 0;
					generate_maze_path(r, c - 1, 0,0,0,1);
				}
			}
			if (moves[i]==3) {			//right
				if (c + 1 <= 9 && maze_path[r][c + 1].visited == -1) {
					maze_path[r][c].right = 0;
					generate_maze_path(r, c + 1, 0,0,1,0);
				}
			}
		}
		return;
	}

	void generate_maze_walls() {
		// build all walls
		for (int r = 0; r < 21; r++){
			for (int c = 0; c < 21; c++){
				maze_walls[r][c] = 1;
			}
		}

		// take down walls
		for (int r = 0; r < 21; r++){
			for (int c = 0; c < 21; c++){
				if (r%2!=0 && c%2!=0){	// if in cell
					// break cell space
					maze_walls[r][c] = 0;
					int mpr = r / 2;
					int mpc = c / 2;
					// break cell walls
					if (maze_path[mpr][mpc].up == 0) maze_walls[r-1][c] = 0;
					if (maze_path[mpr][mpc].down == 0) maze_walls[r+1][c] = 0;
					if (maze_path[mpr][mpc].left == 0) maze_walls[r][c-1] = 0;
					if (maze_path[mpr][mpc].right == 0) maze_walls[r][c+1] = 0;
				}
			}
		}

		// break start and end wall
		maze_walls[1][0] = 0;
		maze_walls[19][20] = 0;
	}

	void generate_maze() {
		// generate maze path and maze walls
		generate_maze_path(0, 0, 0, 0, 0, 0);
		generate_maze_walls();
		print_maze_walls();

		// add walls to wall_list
		wall_list.resize(240);
		int wallcount = 0;

		for (int r = 0; r < 21; r++) {
			for (int c = 0; c < 21; c++) {
				if (maze_walls[r][c] == 1) {
					wall_list[wallcount].brickk = new Brick;
					wall_list[wallcount].loc = vec3(r, 0.0, c);
					wallcount++;
				}
			}
		}
	}

	void draw(GLfloat theta[]) {
		//cout << "maze.draw()" << endl;
		// draw 240 bricks

		//wall_list[0].brickk->draw(theta, vec3(6.0, 0.0, 0.0));

		for (int i = 0; i < 100; i++){
			//go_brick_1.draw(theta, vec3(-4.0, 0.0, 0.0));
			//wall_list[i].brickk->draw(theta, wall_list[i].loc);
			wall_list[i].brickk->draw(theta, wall_list[i].loc);
		}
	}

	void init() {
		for (int i = 0; i < 100; i++){
			wall_list[i].brickk->init_data();
			wall_list[i].brickk->init_VAO();
			wall_list[i].brickk->init_VBO();
			wall_list[i].brickk->init_shader();
			wall_list[i].brickk->init_texture_map();
		}
	}

	void cleanup() {
		for (int i = 0; i < 240; i++) {
			wall_list[i].brickk->cleanup();
		}
	}

	void print_maze_path() {
		for (int r = 0; r < 10; r++){
			for (int c = 0; c < 10; c++){
				cout << maze_path[r][c].visited << " ";
			}
			cout << endl;
		}
	}
	void print_maze_walls() {
		char ch = 178;
		int wallscount = 0;

		for (int r = 0; r < 21; r++) {
			for (int c = 0; c < 21; c++) {
				if (maze_walls[r][c]==1) {
					cout << ch;
					wallscount++;
				}
				else{
					cout << " ";
				}
			}
			cout << endl;
		}
		cout << "wall count: " << wallscount << endl;
	}


};
#endif