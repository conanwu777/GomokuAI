#include "Selector.hpp"

pos Selector::killerAlpha[10];
pos Selector::killerBeta[10];
pos Selector::nxMove;

int Selector::bestMove(Game &g, char c)
{
	pos it;

	for (it.y = 0; it.y < 19; it.y++)
		for (it.x = 0; it.x < 19; it.x++)
			if (!g.board[it.y][it.x])
			{
				Selector::nxMove = it;
				g.move(it);
				return g.score;
			}
	return 0;
}

bool	Selector::tryMove(Game &g, int depth, char c, pos test, int *ret, bool last){
	
	Game t = g;
	if (t.move(test) != -1)
	{
		int tmp = Selector::minimax(t, depth + 1, c, last);
		if ((g.turn != c && tmp < *ret) || (g.turn == c && tmp > *ret))
		{
			*ret = tmp;
			Selector::nxMove = test;
		}

		if (g.turn == c)
		{ //max
			g.alpha = max(g.alpha, *ret);
			killerAlpha[depth] = test;
			if (g.alpha >= g.beta)
				return true;
		}
		else
		{ //min
			g.beta = min(g.beta, *ret);
			killerBeta[depth] = test;
			if (g.alpha >= g.beta)
				return true;
		}
	}
	return false;
}

int Selector::minimax(Game &g, int depth, char c, bool last)
{
	int neg = (c == 'b' ? 1 : -1);
	if (depth >= MAX_DEPTH || g.won || last)
		return neg * g.score / (2 << depth);

	for (int k = 0; k < 6; k++)
		if (g.comp[k])
			last = true;

	int ret = (g.turn != c ? INT_MAX : INT_MIN);

	if (!g.board[killerAlpha[depth].y][killerAlpha[depth].x]
		&& g.adjacent(killerAlpha[depth]))
		if (tryMove(g, depth, c, killerAlpha[depth], &ret, last))
			return ret;

	if (!g.board[killerBeta[depth].y][killerBeta[depth].x]
		&& g.adjacent(killerBeta[depth]))
		if (tryMove(g, depth, c, killerBeta[depth], &ret, last))
			return ret;

	pos iter;
	for (iter.y = 0; iter.y < 19; iter.y++)
		for (iter.x = 0; iter.x < 19; iter.x++)
		{
			if (iter == killerAlpha[depth])
				continue;
			if (iter == killerBeta[depth])
				continue;
			if (g.board[iter.y][iter.x] || !g.adjacent(iter))
				continue ;
			if (tryMove(g, depth, c, iter, &ret, last))
				return ret;
		}
	return ret;
}