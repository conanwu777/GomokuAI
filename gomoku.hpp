#ifndef GOMOKU_HPP
# define GOMOKU_HPP
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <unistd.h>
# include <vector>
# include <unordered_map>
# include "Pos.hpp"

# define MAX_DEPTH 6
# define CUTOFF 8

using namespace std;

class Game;
class Display;

bool operator==(const pos& lhs, const pos& rhs);
pos operator+(const pos& p, const pos& other);
pos operator*(int num, const pos& p);

int minimax(Game *g, int depth, char c, bool last);
bool tryMove(Game *g, int depth, char c, pos test, int *ret, bool last);
extern pos killerAlpha[10];
extern pos killerBeta[10];
extern pos nxMove;
extern bool b;
extern bool w;
extern float mult_b;
extern float mult_w;

#endif