#ifndef SELECTOR_HPP
# define SELECTOR_HPP

#include "Game.hpp"

class Selector {
public:
	static int bestMove(Game &g, char c);
	static int minimax(Game &g, int depth, char c, bool last);
	static bool tryMove(Game &g, int depth, char c, pos test, int *ret, bool last);
	static pos killerAlpha[10];
	static pos killerBeta[10];
	static pos nxMove;
};

#endif