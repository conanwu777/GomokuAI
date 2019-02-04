#ifndef GAME_HPP
# define GAME_HPP
# include "gomoku.hpp"
# include <deque>

class Selector;

class Game {
public:
	int comp[8];
	int alpha;
	int beta;
	char turn;
	char won;
	char trueWon;
	int score;
	int cap_b;
	int cap_w;
	char board[19][19];

	map<pos, Game*, posCompare> nxs;
	Game *pv;
	pos lastMv;

	Game();
	Game(const Game &g);
	Game& operator=(const Game &g);
	~Game();
	Game *aiMove();
	Game *move(pos p);
	char checkWin();
	bool capture(pos p, int x, int y, char opp);
	bool checkCapture(pos p);
	bool inBound(int x, int y) const;
	bool adjacent(pos p) const;
	bool checkValid(pos p);
	int checkThree(pos p, int x, int y);   
	bool checkLineThrees(deque<char> &line, char target);
	char checkLine(int stx, int sty, int incx, int incy);
	void getScore();
};

#endif
