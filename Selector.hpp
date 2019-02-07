#ifndef SELECTOR_HPP
# define SELECTOR_HPP

class Selector {
	Game *game;
	int maxDepth;
	char c;

public:
	pos out;
	Selector();
	Selector(Game *game, char c, int maxDepth);
	~Selector();
	bool	tryMove(int *ret, int depth, pos test, bool last);
	int		minimax(int depth, bool last);

};

#endif