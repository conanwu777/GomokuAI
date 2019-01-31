#include "Selector.hpp"

pos Selector::killerAlpha[10];
pos Selector::killerBeta[10];

int Selector::eval(const Game &g, int depth)
{
	if (g.won)
		return (g.won == 'b' ? INT_MAX : INT_MIN) / (2 << depth);
	int ret = 0;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			if (g.board[i][j] == 'b')
				ret -= abs(i - 9) + abs(j - 9);
			else if (g.board[i][j])
				ret += abs(i - 9) + abs(j - 9);
		}
	ret += 1000000 * g.comp[0] - 1000000 * g.comp[1];
	ret += 100000 * g.comp[2] - 100000 * g.comp[3];
	ret += 10000 * g.comp[4] - 10000 * g.comp[5];
	ret += 1000 * g.cap_b * g.cap_b - 1000 * g.cap_w * g.cap_w;
	return ret / (2 << depth);
}

// void	calcScore(Game &g, char c, int depth){
// 	g.score = eval(g, c, depth);
// }


int	Selector::tryMove(Game &g, int depth, int &x, int &y, char c, int i, int j, int *ret){
	
	Game t = g;
	if (t.move(i, j) != -1)
	{
		int tx, ty;

		int tmp = Selector::minimax(t, depth + 1, tx, ty, c);
		if ((g.turn != c && tmp < *ret) || (g.turn == c && tmp > *ret))
		{
			*ret = tmp;
			x = i;
			y = j;
		}

		if (g.turn == c)
		{ //max
			g.alpha = max(g.alpha, *ret);
			killerAlpha[depth].x = i;
			killerAlpha[depth].y = j;
			if (g.alpha >= g.beta)
				return *ret;
		}
		else
		{ //min
			g.beta = min(g.beta, *ret);
			killerBeta[depth].x = i;
			killerBeta[depth].y = j;
			if (g.alpha >= g.beta)
				return *ret;
		}
	}
	return INT_MIN;
}


int Selector::minimax(Game &g, int depth, int &x, int &y, char c)
{
	int neg = (c == 'b' ? 1 : -1);
	if (depth >= MAX_DEPTH || g.won)
		return neg * eval(g, depth);

	int ret = (g.turn != c ? INT_MAX : INT_MIN);


	if (!g.board[killerAlpha[depth].y][killerAlpha[depth].x] && g.adjacent(killerAlpha[depth].x, killerAlpha[depth].y))
		if (tryMove(g, depth, x, y, c, killerAlpha[depth].x, killerAlpha[depth].y, &ret) == ret)
			return ret;

	if (!g.board[killerBeta[depth].y][killerBeta[depth].x] && g.adjacent(killerBeta[depth].x, killerBeta[depth].y))
		if (tryMove(g, depth, x, y, c, killerBeta[depth].x, killerBeta[depth].y, &ret) == ret)
			return ret;

	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			if (i == killerAlpha[depth].x && j == killerAlpha[depth].y)
				continue;
			if (i == killerBeta[depth].x && j == killerBeta[depth].y)
				continue;
			if (g.board[j][i] || !g.adjacent(i, j))
				continue ;
			if (tryMove(g, depth, x, y, c, i, j, &ret) == ret)
				return ret;
		}
	return ret;
}