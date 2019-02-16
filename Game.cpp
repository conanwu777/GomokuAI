#include "Game.hpp"
#include "Display.hpp"
#include "Selector.hpp"

Game::Game() : turn('b'), won(0), cap_b(0), cap_w(0), pv(NULL),
alpha(INT_MIN), beta(INT_MAX), score_b(0), score_w(0), trueWon(0)
{
	lastMv = {9, 9};
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			board[i][j] = 0;
}

Game& Game::operator=(const Game &g)
{
	trueWon = g.trueWon;
	alpha = g.alpha;
	beta = g.beta;
	turn = g.turn;
	won = g.won;
	score_b = g.score_b;
	score_w = g.score_w;
	cap_b = g.cap_b;
	cap_w = g.cap_w;
	for (int x = 0; x < 19; x++)
		for (int y = 0; y < 19; y++)
			board[y][x] = g.board[y][x];
	for (int i = 0; i < 10; i++)
		comp[i] = g.comp[i];
	return *this;
}

Game::Game(const Game &g)
{
	*this = g;
}

Game::~Game() {}

void Game::getScore()
{
	if (trueWon)
	{
		score_b = (trueWon == 'b' ? 30000000 : -30000000);
		score_w = -score_b;
		return ;
	}
	if (won)
	{
		score_b = (won == 'b' ? 10000000 : -10000000);
		score_w = -score_b;
		return ;
	}
	int base_b = 0;
	int base_w = 0;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			if (board[i][j] == 'b')
				base_b += 8 * (abs(i - 9) + abs(j - 9));
			else if (board[i][j])
				base_w += 8 * (abs(i - 9) + abs(j - 9));

	base_b += 1000000 * comp[0] + 200000 * comp[2]
		+ 10000 * comp[4] + 100 * comp[6] - 1000 * comp[8];
	base_b += 10000 * cap_b + 4000 * cap_b * cap_b * mult_b;

	base_w += 1000000 * comp[1] + 200000 * comp[3]
		+ 10000 * comp[5] + 100 * comp[7] - 1000 * comp[9];
	base_w += 10000 * cap_w + 4000 * cap_w * cap_w * mult_w;

	score_b = base_b - 10 * base_w;
	score_w = base_w - 10 * base_b;
}

Game *Game::move(pos p)
{
	if (nxs.find(p) != nxs.end())
	{
		nxs[p]->turn = (turn == 'b' ? 'w' : 'b');
		nxs[p]->alpha = alpha;
		nxs[p]->beta = beta;
		return nxs[p];
	}
	if (!inBound(p.x, p.y) || board[p.y][p.x])
		return NULL;
	Game *ret = new Game(*this);
	ret->board[p.y][p.x] = turn;
	bool capture = ret->checkCapture(p);
	if (!ret->trueWon && !ret->won)
		ret->won = ret->checkWin();
	else if (!ret->trueWon && ret->checkWin())
	{
		ret->trueWon = ret->won;
		ret->won = false;
	}
	else
		ret->won = false;
	if (!capture && !ret->checkValid(p))
	{
		delete ret;
		return NULL;
	}
	nxs[p] = ret;
	ret->nxs.clear();
	ret->pv = this;
	ret->lastMv = p;
	ret->turn = (turn == 'b' ? 'w' : 'b');
	ret->getScore();
	return ret;
}

Game *Game::aiMove()
{
	if (guessMv.find(lastMv) != guessMv.end())
	{
		cout << ORANGE << "Depth searched : " << WHITE << searched << endl;
		return move(guessMv[lastMv]);
	}

	pos best;
	int i = 2;
	int st = Display::centisec;
	while (i < 20)
	{
		alpha = INT_MIN;
		beta = INT_MAX;
		Selector sele(this, turn, 'a', i, st);
		if (sele.minimax(0, false) != -1)
			best = sele.out;
		else
			break ;
		i += 2;
	}
	Game *ret = move(best);
	cout << ORANGE << "Depth searched : " << WHITE << i << endl;
	return ret;
}

void Game::freeGames(pos p, bool b)
{
	for (auto it = nxs.begin(); it != nxs.end(); it++)
	{
		if (b && it->first == p)
			continue ;
		it->second->freeGames(p, false);
		delete it->second;
		nxs.erase(it);
	}
}
