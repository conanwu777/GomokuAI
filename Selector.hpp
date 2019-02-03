#ifndef SELECTOR_HPP
# define SELECTOR_HPP

# include "Game.hpp"
# include "Display.hpp"
# include <vector>

struct Move {
	pos p;
	int score;
	char turn;
	bool isCapture;
	bool isThree;
	bool isFour;
};

class Selector {
public:
	static int minimax(Game &g, int depth, char c, bool last);
	static bool tryMove(Game &g, int depth, char c, pos test, int *ret, bool last);
	static int	rankPlayerMoves(Display &d, Game &g);
	static Move createMove(Game &g, Game &t, pos p);
	static pos killerAlpha[10];
	static pos killerBeta[10];
	static pos nxMove;
	static vector<Move> playerMoves;
};

#endif