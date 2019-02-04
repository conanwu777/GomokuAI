#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

pos killerAlpha[10];
pos killerBeta[10];
pos nxMove;

bool	tryMove(Game *g, int depth, char c, pos test,
int *ret, bool last)
{	
	if (!g->move(test))
		return false;
	Game *t = g->nxs[test];
	int tmp = minimax(t, depth + 1, c, last);
	if ((g->turn != c && tmp < *ret) || (g->turn == c && tmp > *ret))
	{
		*ret = tmp;
		if (depth == 0)
			nxMove = test;
		if (g->turn == c)
		{
			g->alpha = max(g->alpha, *ret);
			killerAlpha[depth] = test;
			if (g->alpha >= g->beta)
				return true;
		}
		else
		{
			g->beta = min(g->beta, *ret);
			killerBeta[depth] = test;
			if (g->alpha >= g->beta)
				return true;
		}
	}
	return false;
}

int minimax(Game *g, int depth, char c, bool last)
{
	g->alpha = INT_MIN;
	g->beta = INT_MAX;
	int neg = (c == 'b' ? 1 : -1);
	if (depth >= MAX_DEPTH || g->won || last)
		return neg * g->score / (2 << depth);
	for (int k = 0; k < 6; k++)
		if (g->comp[k])
			last = true;
	g->rankMoves();
	int ret = (g->turn != c ? INT_MAX : INT_MIN);
	if (!g->board[killerAlpha[depth].y][killerAlpha[depth].x]
		&& g->adjacent(killerAlpha[depth]))
		if (tryMove(g, depth, c, killerAlpha[depth], &ret, last))
			return ret;
	if (!g->board[killerBeta[depth].y][killerBeta[depth].x]
		&& g->adjacent(killerBeta[depth]))
		if (tryMove(g, depth, c, killerBeta[depth], &ret, last))
			return ret;
	for (int i = 0; i < CUTOFF && i < g->moves.size(); i++)
		if (tryMove(g, depth, c, g->moves[i], &ret, last))
			return ret;
	return ret;
}