#include "Game.hpp"
#include "Display.hpp"

bool Game::inBound(int x, int y) const
{
	return (x >= 0 && y >= 0 && x < 19 && y < 19);
}

bool Game::adjacent(pos p) const
{
	if (inBound(p.x - 1, p.y) && board[p.y][p.x - 1])
		return true;
	if (inBound(p.x, p.y - 1) && board[p.y - 1][p.x])
		return true;
	if (inBound(p.x + 1, p.y) && board[p.y][p.x + 1])
		return true;
	if (inBound(p.x, p.y + 1) && board[p.y + 1][p.x])
		return true;
	if (inBound(p.x - 1, p.y - 1) && board[p.y - 1][p.x - 1])
		return true;
	if (inBound(p.x + 1, p.y + 1) && board[p.y + 1][p.x + 1])
		return true;
	if (inBound(p.x + 1, p.y - 1) && board[p.y - 1][p.x + 1])
		return true;
	if (inBound(p.x - 1, p.y + 1) && board[p.y + 1][p.x - 1])
		return true;
	return false;
}

Game::Game() : turn('b'), won(0), cap_b(0), cap_w(0),
alpha(INT_MIN), beta(INT_MAX), score(0), trueWon(0), pv(NULL)
{
	lastMv = {-1, -1};
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
	nxs = g.nxs;
	pv = g.pv;
	lastMv = g.lastMv;
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

char Game::checkLine(int stx, int sty, int incx, int incy)
{
	bool prevOpen = false;
	bool open = false;
	bool first = true;
	int curBlock = 0;
	char curPlayer = 0;
	int prevBlock = 0;
	char prevPlayer = -1;
	while (inBound(stx, sty))
	{
		if (board[sty][stx] && !first &&
			board[sty - incy][stx - incx] == board[sty][stx])
			curBlock++;
		else if (board[sty][stx])
		{
			if ((open && curBlock == 4) || (prevBlock && curBlock &&
				curBlock + prevBlock == 4 && curPlayer == prevPlayer))
				comp[(curPlayer == 'b' ? 2 : 3)]++;
			else if ((open && curBlock == 3) || (prevBlock && curBlock &&
				curBlock + prevBlock == 3 && curPlayer == prevPlayer))
				comp[(curPlayer == 'b' ? 6 : 7)]++;
			open = true;
			if (first || board[sty - incy][stx - incx])
				open = false;
			if (curPlayer != board[sty][stx])
				prevBlock = 0;
			curBlock = 1;
			curPlayer = board[sty][stx];
		}
		else
		{
			if (open && curBlock == 4)
				comp[(curPlayer == 'b' ? 0 : 1)]++;
			else if (curBlock == 4 || (prevBlock && curBlock &&
				curBlock + prevBlock == 4 && curPlayer == prevPlayer))
				comp[(curPlayer == 'b' ? 2 : 3)]++;
			else if (open && (curBlock == 3 || (prevOpen && prevBlock && curBlock
				&& curBlock + prevBlock == 3 && curPlayer == prevPlayer)))
				comp[(curPlayer == 'b' ? 4 : 5)]++;
			else if (open && (curBlock == 2 || (prevBlock && curBlock &&
				curBlock + prevBlock == 2  && curPlayer == prevPlayer)))
				comp[(curPlayer == 'b' ? 6 : 7)]++;
			prevBlock = curBlock;
			prevPlayer = curPlayer;
			prevOpen = open;
			open = true;
			curBlock = 0;
		}
		if (curBlock >= 5)
			return curPlayer;
		stx += incx;
		sty += incy;
		first = false;
	}
	return 0;
}

char Game::checkWin()
{
	char ret = 0;

	bzero(&comp[0], sizeof(int) * 8);
	for (int i = 0; i < 19; i++)
	{
		if ((ret = checkLine(0, i, 1, 0)))
			return ret;
		if ((ret = checkLine(i, 0, 0, 1)))
			return ret;
		if ((ret = checkLine(i, 0, 1, 1)))
			return ret;
		if (i > 0 && (ret = checkLine(0, i, 1, 1)))
			return ret;
		if ((ret = checkLine(i, 0, -1, 1)))
			return ret;
		if (i > 0 && (ret = checkLine(i, 18, 1, -1)))
			return ret;
	}
	return 0;
}

bool Game::capture(pos p, int dx, int dy, char opp)
{
	pos d;
	d.x = dx;
	d.y = dy;
	if (inBound(p.x + 3 * d.x, p.y + 3 * d.y)
		&& board[p.y + dy][p.x + dx] == opp
		&& board[p.y + 2 * dy][p.x + 2 * dx] == opp
		&& board[p.y + 3 * dy][p.x + 3 * dx] == turn)
	{
		board[p.y + dy][p.x + dx] = 0;
		board[p.y + 2 * dy][p.x + 2 * dx] = 0;
		return true;
	}
	return false;
}

bool Game::checkCapture(pos p)
{
	int c = 0;
	char opp = (turn == 'b' ? 'w' : 'b');
	if (capture(p, 1, 0, opp))
		c++;
	if (capture(p, 0, 1, opp))
		c++;
	if (capture(p, -1, 0, opp))
		c++;
	if (capture(p, 0, -1, opp))
		c++;
	if (capture(p, 1, 1, opp))
		c++;
	if (capture(p, 1, -1, opp))
		c++;
	if (capture(p, -1, 1, opp))
		c++;
	if (capture(p, -1, -1, opp))
		c++;
	(turn == 'b' ? cap_b : cap_w) += c;
	if ((turn == 'b' ? cap_b : cap_w) >= 5)
		trueWon = turn;
	return c != 0;
}

bool Game::checkLineThrees(deque<char> &line, char target)
{
	bool leftOpen = false;
	int count = 0;
	bool hole = false;
	int num = 0;
	for (auto i = line.begin(); i != line.end() ; i++)
		if (*i == 0)
		{
			if (!leftOpen)
				leftOpen = true;
			else if (count == 3)
				return true;
			else if (hole == false)
				hole = true;
			else
				return 0;
		}
		else if (*i == target)
			count += leftOpen ? 1 : 0;
		else
			if (leftOpen)
				return 0;
	return 0;
}

int Game::checkThree(pos p, int x, int y)
{
	bool emptyLeft = false;
	bool emptyRight = false;
	deque<char> cur;
	pos off;

	off.x = x;
	off.y = y;
	for (int i = -4; i < 5; i++)
	{
		if (cur.size() >= 6)
			cur.pop_front();
		if (inBound(p.x + i * off.x, p.y + i * off.y))
			cur.push_back(board[p.y + i * off.y][p.x + i * off.x]);
		if (checkLineThrees(cur, turn))
			return 1;
	}
	return 0;
}

bool Game::checkValid(pos p)
{
	int count = 0;
	count += checkThree(p, 1, 0);
	count += checkThree(p, 0, 1);
	count += checkThree(p, 1, 1);
	count += checkThree(p, 1, -1);
	if (count >= 2)
		return false;
	return true;
}

void Game::getScore()
{
	if (trueWon)
	{
		score = (won == 'b' ? 2000000000 : -2000000000);
		return ;
	}
	else if (won){
		score = (won == 'b' ? 1000000000 : -1000000000);
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
		ret += 20000 + 4000 * (cap_b * cap_b);
	if (cap_w)
		ret -= 20000 + 4000 * (cap_w * cap_w);
	score = ret;
}

Game *Game::move(pos p)
{

	bool capture = true;

	if (nxs.find(p) != nxs.end())
		return nxs[p];
	if (!inBound(p.x, p.y) || board[p.y][p.x]){
		return NULL;
	}
	Game *ret = new Game(*this);
	ret->board[p.y][p.x] = turn;
	capture = ret->checkCapture(p);
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
	ret->pv = this;
	ret->lastMv = p;
	ret->getScore();
	cout << ret->score << endl;
	ret->turn = (turn == 'b' ? 'w' : 'b');
	return ret;
}

Game *Game::aiMove()
{
	alpha = INT_MIN;
	beta = INT_MAX;
	Game g = *this;
	minimax(g, 0, turn, false);
	return move(nxMove);
}
