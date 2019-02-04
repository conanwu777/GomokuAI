#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

pos killerAlpha[10];
pos killerBeta[10];
pos nxMove;
vector<Move> playerMoves;

// bool compareMove(Move &m1, Move &m2) 
// {
// 	if (m1.turn == 'b')
// 		return m1.score >= m2.score;
// 	else
// 		return m1.score < m2.score;
// }

// Move createMove(Game &g, Game &t, pos p)
// {
// 	Move m;

// 	m.p = p;
// 	m.score = t.score;
// 	m.turn = g.turn;
// 	m.isCapture = (t.cap_b > g.cap_b || t.cap_w > g.cap_w);
// 	m.isThree = (t.comp[4] > g.comp[4] || t.comp[5] > g.comp[5]);
// 	m.isFour = (t.comp[2] > g.comp[2] || t.comp[3] > g.comp[3]);
// 	return m;
// }

// int rankPlayerMoves(Display &d, Game &g)
// {
// 	if (d.isAITurn())
// 		return 0;
// 	playerMoves.clear();
// 	pos p;
// 	for (p.y = 0; p.y < 19; p.y++)
// 		for (p.x = 0; p.x < 19; p.x++)
// 		{
// 			if (g.board[p.y][p.x] || !g.adjacent(p))
// 				continue ;
// 			Game *t = g.move(p);
// 			if (t == NULL)
// 				continue ;
// 			playerMoves.push_back(createMove(g, *t, p));
// 		}
// 	sort(playerMoves.begin(), playerMoves.end(), compareMove);
// 	return 1;
// }

bool	tryMove(Game &g, int depth, char c, pos test,
int *ret, bool last)
{	
	if (g.move(test) != NULL)
	{
		Game *t = g.nxs[test];
		int tmp = minimax(*t, depth + 1, c, last);
		if ((g.turn != c && tmp < *ret) || (g.turn == c && tmp > *ret))
		{
			*ret = tmp;
			if (depth == 0){
				nxMove = test;
			}
			if (g.turn == c)
			{
				g.alpha = max(g.alpha, *ret);
				killerAlpha[depth] = test;
				if (g.alpha >= g.beta)
					return true;
			}
			else
			{
				g.beta = min(g.beta, *ret);
				killerBeta[depth] = test;
				if (g.alpha >= g.beta)
					return true;
			}
		}
	}
	return false;
}

int minimax(Game &g, int depth, char c, bool last)
{
	g.alpha = INT_MIN;
	g.beta = INT_MAX;
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
			if (!g.board[iter.y][iter.x] && g.adjacent(iter))
			{
				if (iter == killerAlpha[depth])
					continue;
				if (iter == killerBeta[depth])
					continue;
				if (tryMove(g, depth, c, iter, &ret, last))
					return ret;
			}
		}
	return ret;
}