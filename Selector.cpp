#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

pos killerAlpha[10];
pos killerBeta[10];
pos nxMove;

void Game::rankMoves()
{
	if (!moves.empty())
		return ;
	pos iter;
	for (iter.y = 0; iter.y < 19; iter.y++)
		for (iter.x = 0; iter.x < 19; iter.x++)
			if (!board[iter.y][iter.x] && adjacent(iter))
			{
				if (!move(iter))
					continue ;
				moves.push_back(iter);
			}
	int neg = (turn == 'w' ? -1 : 1);
	if (moves.size())
		for (int i = 0; i < moves.size() - 1; i++)
			for (int j = i + 1; j < moves.size(); j++)
				if (neg * nxs[moves[i]]->score < neg * nxs[moves[j]]->score)
				{
					pos t = moves[i];
					moves[i] = moves[j];
					moves[j] = t;
				}
}

bool	Game::tryMove(int depth, char c, pos test,
int *ret, bool last)
{	

	if (!move(test))
		return false;
	Game *t = nxs[test];
	int tmp = t->minimax(depth + 1, c, last);
	if ((turn != c && tmp < *ret) || (turn == c && tmp > *ret))
	{
		*ret = tmp;
		if (depth == 0)
			nxMove = test;
		if (turn == c)
		{
			alpha = max(alpha, *ret);
			killerAlpha[depth] = test;
			if (alpha >= beta){
				return true;
			}
		}
		else
		{
			beta = min(beta, *ret);
			killerBeta[depth] = test;
			if (alpha >= beta){

				return true;
			}
		}
	}
	return false;
}

int Game::minimax(int depth, char c, bool last)
{
// static int counter = 0;
// cout << "Minmax : " << counter << " | " << depth << ", " << c << endl;
// counter++;
	int neg = (c == 'b' ? 1 : -1);
	if (depth >= MAX_DEPTH || won || last)
		return neg * score / (2 << depth);
	for (int k = 0; k < 6; k++)
		if (comp[k])
			last = true;
	rankMoves();
	int ret = (turn != c ? INT_MAX : INT_MIN);
	if (!board[killerAlpha[depth].y][killerAlpha[depth].x]
		&& adjacent(killerAlpha[depth]))
		if (tryMove(depth, c, killerAlpha[depth], &ret, last))
			return ret;
	if (!board[killerBeta[depth].y][killerBeta[depth].x]
		&& adjacent(killerBeta[depth]))
		if (tryMove(depth, c, killerBeta[depth], &ret, last))
			return ret;
	for (int i = 0; i < CUTOFF && i < moves.size(); i++)
		if (tryMove(depth, c, moves[i], &ret, last))
			return ret;
	return ret;
}
