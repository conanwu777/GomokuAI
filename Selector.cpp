#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"
#include "Selector.hpp"

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

Selector::Selector() {}

Selector::Selector(Game *game, char c, int md)
: game(game), c(c), out({-1, -1}), maxDepth(md) {}

Selector::~Selector() {}

bool	Selector::tryMove(int* ret, int depth, pos test, bool last)
{	
	if (!game->move(test))
		return false;
	Game *save = game;
	game = game->nxs[test];

	int tmp = minimax(depth + 1, last);
	game = save;
	if ((game->turn != c && tmp < *ret) || (game->turn == c && tmp > *ret))
	{
		*ret = tmp;
		if (depth == 0)
			out = test;
		if (game->turn == c)
		{
			game->alpha = max(game->alpha, *ret);
			killerAlpha[depth] = test;
			if (game->alpha >= game->beta)
				return true;
		}
		else
		{
			game->beta = min(game->beta, *ret);
			killerBeta[depth] = test;
			if (game->alpha >= game->beta)
				return true;
		}
	}
	return false;
}

int Selector::minimax(int depth, bool last)
{
	int neg = (c == 'b' ? 1 : -1);
	if (depth >= maxDepth || game->trueWon || last)
		return neg * game->score;
	for (int k = 0; k < 6; k++)
		if (game->comp[k])
			last = true;
	game->rankMoves();
	int ret = (game->turn != c ? INT_MAX : INT_MIN);
	if (!game->board[killerAlpha[depth].y][killerAlpha[depth].x]
		&& game->adjacent(killerAlpha[depth]))
		if (tryMove(&ret, depth, killerAlpha[depth], last))
			return (ret >> 1) + neg * game->score;
	if (!game->board[killerBeta[depth].y][killerBeta[depth].x]
		&& game->adjacent(killerBeta[depth]))
		if (tryMove(&ret, depth, killerBeta[depth], last))
			return (ret >> 1) + neg * game->score;
	for (int i = 0; i < CUTOFF && i < game->moves.size(); i++)
		if (tryMove(&ret, depth, game->moves[i], last))
			return (ret >> 1) + neg * game->score;
	return (ret >> 1) + neg * game->score;
}
