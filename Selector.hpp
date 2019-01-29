#ifndef SELECTOR_HPP
# define SELECTOR_HPP

#include "Game.hpp"
#include <queue>
#include <array>

struct Comparator { 
    bool operator() (Game const& p1, Game const& p2) 
    { 
        return p1.score > p2.score; 
    } 
}; 

class Selector {
public:
	static int runLevel(Game &g, int depth, int &x, int &y, char c);
	static int minimax(Game &g, int depth, int &x, int &y, char c);
	static array<priority_queue<Game, vector<Game>, Comparator>, 10> pq;

//priority queue not in use curently. need to add to queue after clac score then pick top 15 and run what is in minimax function after calc score call. love ya. Bye

};

#endif