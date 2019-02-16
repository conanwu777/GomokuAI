#ifndef GOMOKU_HPP
# define GOMOKU_HPP
# include <iostream>
# include <ctime>
# include <unistd.h>
# include <vector>
# include <unordered_map>
# include "Pos.hpp"

# define RED "\e[1;38;2;225;20;20m"
# define WHITE "\e[1;38;2;255;251;214m"
# define YELLO "\e[1;38;2;255;200;0m"
# define ORANGE "\e[1;38;2;255;120;10m"
# define GREY "\e[1;38;2;120;100;100m"

# define CUTOFF 15
# define GUESSNUM 10
# define T_DELTA 50

using namespace std;

class Game;
class Display;

bool	operator==(const pos& lhs, const pos& rhs);
pos		operator+(const pos& p, const pos& other);
pos		operator*(int num, const pos& p);

extern pos killerAlpha[10];
extern pos killerBeta[10];
extern pos nxMove;
extern unordered_map<pos, pos, Hash> guessMv;
extern bool b;
extern bool w;
extern float mult_b;
extern float mult_w;
extern mutex mtx;
extern bool mutexRequested;
extern int searched;

#endif
