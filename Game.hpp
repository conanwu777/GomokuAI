#ifndef GAME_HPP
# define GAME_HPP
# include "gomoku.hpp"
# include <deque>

# define MAX_DEPTH 5

class Game {
public:
	int comp[6];
	int alpha;
    int beta;
	char ai;
	char turn;
	char won;
	int cap_b;
	int cap_w;
	char board[19][19];
	Game();
	Game(const Game &g);
	Game& operator=(const Game &g);
	~Game();
	int aiMove();
	int move(int x, int y);
	char checkWin();
	bool capture(int x, int y, int dx, int dy, char opp);
	bool checkCapture(int x, int y);
	bool inBound(int x, int y) const;
	bool adjacent(int x, int y) const;
	bool checkValid(int, int);
	int checkThree(int x, int y, int xOff, int yOff);   
	bool checkLineThrees(deque<char> &line, char target);
	char checkLine(int stx, int sty, int incx, int incy);
};

int eval(const Game &, int depth);
int minimax(Game &g, int depth, int &x, int &y, char c);

#endif
