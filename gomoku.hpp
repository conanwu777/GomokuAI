#ifndef GOMOKU_HPP
# define GOMOKU_HPP
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <unistd.h>
# include <vector>
# include <map>

# define MAX_DEPTH 4
# define CUTOFF 15

using namespace std;

class Game;
class Display;
struct pos
{
	int x;
	int y;
	pos operator=(const pos& p);
	pos(const pos &p);
	pos(){
		x = 0;
		y = 0;
	}
	pos(int nx, int ny){
		x = nx;
		y = ny;
	}
};

struct posCompare
{
	bool operator() (const pos& lhs, const pos& rhs) const
	{
		return lhs.x * 20 + lhs.y < rhs.x * 20 + rhs.y;
	}
};

bool operator==(const pos& lhs, const pos& rhs);
pos operator+(const pos& p, const pos& other);
pos operator*(int num, const pos& p);

int minimax(Game *g, int depth, char c, bool last);
bool tryMove(Game *g, int depth, char c, pos test, int *ret, bool last);
extern pos killerAlpha[10];
extern pos killerBeta[10];
extern pos nxMove;
extern bool b;
extern bool w;
extern float mult_b;
extern float mult_w;

#endif