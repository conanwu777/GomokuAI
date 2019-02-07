#include "Game.hpp"
#include "Display.hpp"
#include "Selector.hpp"

Game::Game() : turn('b'), won(0), cap_b(0), cap_w(0),
alpha(INT_MIN), beta(INT_MAX), score(0), trueWon(0), pv(NULL)
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
	score = g.score;
	cap_b = g.cap_b;
	cap_w = g.cap_w;
	for (int x = 0; x < 19; x++)
		for (int y = 0; y < 19; y++)
			board[y][x] = g.board[y][x];
	for (int i = 0; i < 8; i++)
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
		score = (won == 'b' ? 30000000 : -30000000);
		return ;
	}
	else if (won)
	{
		score = (won == 'b' ? 10000000 : -10000000);
		return;
	}
	int ret = 0;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			if (board[i][j] == 'b')
				ret -= 8 * (abs(i - 9) + abs(j - 9));
			else if (board[i][j])
				ret += 8 * (abs(i - 9) + abs(j - 9));
		}
	int coeb = (turn == 'b' ? 1 : 10);
	int coew = (turn == 'w' ? 1 : 10);
	ret += coeb * 1000000 * comp[0] - coew * 1000000 * comp[1];
	ret += coeb * 200000 * comp[2] - coew * 200000 * comp[3];
	ret += coeb * 10000 * comp[4] - coew * 10000 * comp[5];
	ret += 1000 * comp[6] - 1000 * comp[7];
	if (cap_b)
		ret += 20000 + 6000 * (cap_b * cap_b);
	if (cap_w)
		ret -= 20000 + 6000 * (cap_w * cap_w);
	score = ret;

// cout << "getting score (" << lastMv.x << ", " << lastMv.y << ") : " << endl;
// cout << comp[0] << ", " << comp[1] << " | " << comp[2] << ", " << comp[3] <<
// " | " << comp[4] << ", " << comp[5] << " | " << comp[6] << ", " << comp[7] << endl;
// cout << cap_b << "," << cap_w << " | " << score << endl;

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
// cout << lastMv.x << ", " << lastMv.y << " -> " << p.x << ", " << p.y << endl; 
	ret->nxs.clear();
	ret->pv = this;
	ret->lastMv = p;
	ret->getScore();
	ret->turn = (turn == 'b' ? 'w' : 'b');
	return ret;
}

Game *Game::aiMove()
{
	if (guessMv.find(lastMv) != guessMv.end())
		return move(guessMv[lastMv]);
	cout << "RUNNING MOVE\n";
	alpha = INT_MIN;
	beta = INT_MAX;
	// pos bestMove;
	Selector sele(this, turn, MAX_DEPTH);
	sele.minimax(0, false);
	Game *ret = move(sele.out);
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
