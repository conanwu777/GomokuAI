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
	if (moves.size())
		for (int i = 0; i < moves.size() - 1; i++)
			for (int j = i + 1; j < moves.size(); j++)
				if ((turn == 'b' ? nxs[moves[i]]->score_b : nxs[moves[i]]->score_w)
				 < (turn == 'b' ? nxs[moves[j]]->score_b : nxs[moves[j]]->score_w))
				{
					pos t = moves[i];
					moves[i] = moves[j];
					moves[j] = t;
				}
}

Selector::Selector() {}

Selector::Selector(Game *game, char c, char th, int md, int st)
: game(game), c(c), th(th), out({-1, -1}), maxDepth(md), st(st)
{
	// cout << RED << "Selector to maxmize " << (c == 'b' ? "Black" : "White") << endl;
}

Selector::~Selector() {}

bool	Selector::tryMove(int* ret, int depth, pos test, bool last)
{	
	if (th == 'p' && mutexRequested)
		return 0;
	if (th == 'a' && Display::centisec - st >= T_DELTA)
		return 0;
	if (!game->move(test))
		return 0;
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
				return 1;
		}
		else
		{
			game->beta = min(game->beta, *ret);
			killerBeta[depth] = test;
			if (game->alpha >= game->beta)
				return 1;
		}
	}
	return 0;
}

int Selector::minimax(int depth, bool last)
{
	if (th == 'p' && mutexRequested)
		return -1;
	if (th == 'a' && Display::centisec - st >= T_DELTA)
		return -1;
	int score = (c == 'b' ? game->score_b : game->score_w);
	if (depth >= maxDepth || game->trueWon || last)
		return score;
	for (int k = 0; k < 6; k++)
		if (game->comp[k])
			last = true;
	if (game->moves.size() == 0)
		game->rankMoves(); 
	int ret = (game->turn != c ? INT_MAX : INT_MIN);
	if (!game->board[killerAlpha[depth].y][killerAlpha[depth].x]
		&& game->adjacent(killerAlpha[depth]))
		if (tryMove(&ret, depth, killerAlpha[depth], last))
			return (th == 'a' && Display::centisec - st >= T_DELTA ? -1 : ret);
	if (!game->board[killerBeta[depth].y][killerBeta[depth].x]
		&& game->adjacent(killerBeta[depth]))
		if (tryMove(&ret, depth, killerBeta[depth], last))
			return (th == 'a' && Display::centisec - st >= T_DELTA ? -1 : ret);
	for (int i = 0; i < CUTOFF && i < game->moves.size(); i++)
		if (tryMove(&ret, depth, game->moves[i], last))
			return (th == 'a' && Display::centisec - st >= T_DELTA ? -1 : ret);
	return (th == 'a' && Display::centisec - st >= T_DELTA ? -1 : ret);
}
