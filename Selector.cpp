#include "Selector.hpp"

	array<priority_queue<Game, vector<Game>, Comparator>, 10> Selector::pq;


int eval(const Game &g, char c, int depth)
{
	// if (g.won)
		// return (g.won == c ? INT_MAX : INT_MIN) / (2 << depth);
	int ret = 0;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			if (g.board[i][j] == c)
				ret -= i + j;
	return ret;
}


void	calcScore(Game &g, char c, int depth){
	g.score = eval(g, c, depth);
}

int Selector::runLevel(Game &g, int depth, int &x, int &y, char c){
	int ret = INT_MIN;
	for (int i = 0; i < 5 && i < pq[depth].size(); i++){
		Game t = pq[depth].top();
		pq[depth].pop();
		int tx, ty;
		int tmp = minimax(t, depth + 1, tx, ty, c);
		if (ret == INT_MIN || (g.turn != c && tmp < ret) || (g.turn == c && tmp > ret))
		{
			ret = tmp;
			x = tx;
			y = ty;
		}

		if (g.turn == c){ //max
			g.alpha = max(g.alpha, ret);
			if (g.alpha >= g.beta){
				return ret;
			}
		}
		else{ //min
			g.beta = min(g.beta, ret);
			if (g.alpha >= g.beta){
				return ret;
			}
		}
	}
	while (pq[depth].empty() != true){
		pq[depth].pop();
	}
	return ret;
}

int Selector::minimax(Game &g, int depth, int &x, int &y, char c)
{
	if (depth >= MAX_DEPTH || g.won)
		return g.score;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			if (g.board[j][i] || !g.adjacent(i, j))
				continue ;
			Game t = g;
			if (t.move(i, j) != -1)
			{
				calcScore(t, t.turn, depth + 1); //might be t.turn, might be depth.
				pq[depth].push(t);
			}
		}
	return runLevel(g, depth, x, y, c);
}