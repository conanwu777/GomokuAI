#ifndef SELECTOR_HPP
# define SELECTOR_HPP

class Selector {
	Game *game;
	int maxDepth;
	char c;
	char th;
	int st;
public:
	pos out;
	Selector();
	Selector(Game *game, char c, char th, int maxDepth, int st);
	~Selector();
	bool	tryMove(int *ret, int depth, pos test, bool last);
	int		minimax(int depth, bool last);

};

#endif