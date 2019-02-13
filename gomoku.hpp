#ifndef GOMOKU_HPP
# define GOMOKU_HPP
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <unistd.h>
# include <vector>
# include <unordered_map>
# include "Pos.hpp"

# define MAX_DEPTH 4
# define CUTOFF 20
# define GUESSNUM 10

using namespace std;

class Game;
class Display;

bool operator==(const pos& lhs, const pos& rhs);
pos operator+(const pos& p, const pos& other);
pos operator*(int num, const pos& p);

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

#endif