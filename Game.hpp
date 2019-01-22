#ifndef GAME_HPP
# define GAME_HPP
# include "gomoku.hpp"
# include <deque>

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
	bool checkCapture(int x, int y);
    bool inBound(int x, int y);
    bool checkValid(int, int);
    int checkThree(int x, int y, int xOff, int yOff);   
    bool checkLineThrees(deque<char> &line, char target);
};

#endif
