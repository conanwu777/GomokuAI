#ifndef SELECTOR_HPP
# define SELECTOR_HPP

#include "Game.hpp"


struct pos{
	int x;
	int y;
};

class Selector {
public:
	static int runLevel(Game &g, int depth, int &x, int &y, char c);
	static int minimax(Game &g, int depth, int &x, int &y, char c);
	static int eval(const Game &g, int depth);
	static int tryMove(Game &g, int depth, int &x, int &y, char c, int i, int j, int *ret);
	static pos killerAlpha[10];
	static pos killerBeta[10];
};

#endif