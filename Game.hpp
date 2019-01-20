#ifndef GAME_HPP
# define GAME_HPP
# include "gomoku.hpp"

class Game {
public:
    char turn;
    char won;
    char board[19][19];
    Game();
    ~Game();
    int move(int x, int y);
    char checkWin();

};

#endif
