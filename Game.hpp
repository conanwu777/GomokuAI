#ifndef GAME_HPP
# define GAME_HPP
# include "gomoku.hpp"

class Game {
public:
    char turn;
    char won;
    int cap_b;
    int cap_w;
    char board[19][19];
    Game();
    ~Game();
    int move(int x, int y);
    char checkWin();
    bool capture(int x, int y, int dx, int dy, char opp);
	void checkCapture(int x, int y);
    bool inBound(int x, int y);
    bool checkThree(int x, int y, int dx, int dy);
    bool doubleThree(int x, int y);
};

#endif
