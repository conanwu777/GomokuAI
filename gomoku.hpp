#ifndef GOMOKU_HPP
# define GOMOKU_HPP
# include <iostream>
# include <cstdlib>
# include <ctime>

using namespace std;

struct pos {
	int x;
	int y;
};

bool operator==(const pos& lhs, const pos& rhs);
pos operator+(const pos& p, const pos& other);
pos operator*(int num, const pos& p);

#endif