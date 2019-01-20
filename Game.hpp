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
	void checkCapture(int x, int y);
};

#endif
