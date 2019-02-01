#include "Game.hpp"
#include "Selector.hpp"
#include <chrono>

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
alpha(INT_MIN), beta(INT_MAX), score(0)
{
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			board[i][j] = 0;
}

Game& Game::operator=(const Game &g)
{
	alpha = g.alpha;
	beta = g.beta;
	ai = g.ai;
	turn = g.turn;
	won = g.won;
	score = g.score;
	cap_b = g.cap_b;
	cap_w = g.cap_w;
	for (int x = 0; x < 19; x++)
		for (int y = 0; y < 19; y++)
			board[y][x] = g.board[y][x];
	for (int i = 0; i < 6; i++)
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
	bool open = false;
	bool first = true;
	int c = 0;
	char cur;
	while (inBound(stx, sty))
	{
		if (board[sty][stx] && !first
			&& board[sty - incy][stx - incx] == board[sty][stx])
			c++;
		else if (board[sty][stx])
		{
			if (open && c == 4)
				comp[(cur == 'b' ? 2 : 3)]++;
			open = true;
			if (first || board[sty - incy][stx - incx])
				open = false;
			c = 1;
			cur = board[sty][stx];
		}
		else
		{
			if (open && c == 4)
				comp[(cur == 'b' ? 0 : 1)]++;
			else if (c == 4)
				comp[(cur == 'b' ? 2 : 3)]++;
			if (open && c == 3)
				comp[(cur == 'b' ? 4 : 5)]++;
			open = true;
			c = 0;
		}
		if (c >= 5)
			return cur;
		stx += incx;
		sty += incy;
		first = false;
	}
	return 0;
}

char Game::checkWin()
{
	char ret = 0;

	bzero(&comp[0], sizeof(int) * 6);
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
	(turn == 'b' ? cap_b : cap_w) += 2 * c;
	if ((turn == 'b' ? cap_b : cap_w) >= 10)
		won = turn;
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
			cur.push_back(board[y + i * off.y][x + i * off.x]);
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
	if (won)
	{
		score = (won == 'b' ? 10000000 : -10000000);
		return ;
	}
	int ret = 0;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			if (board[i][j] == 'b')
				ret -= abs(i - 9) + abs(j - 9);
			else if (board[i][j])
				ret -= abs(i - 9) + abs(j - 9);
		}
	int coeb = (turn == 'b' ? 1 : 2);
	int coew = (turn == 'w' ? 1 : 2);
	ret += coeb * 1000000 * comp[0] - coew * 1000000 * comp[1];
	ret += coeb * 100000 * comp[2] - coew * 100000 * comp[3];
	ret += coeb * 10000 * comp[4] - coew * 10000 * comp[5];
	ret += coeb * 5000 * pow(cap_b, 6) - coew * 5000 * pow(cap_w, 6);
	score = ret;
}

int Game::move(pos p)
{
	bool capture = true;

	if (!inBound(p.x, p.y) || board[p.y][p.x])
		return -1;
	board[p.y][p.x] = turn;
	won = checkWin();
	if (!won)
		capture = checkCapture(p);
	if (!capture)
		if (!checkValid(p))
		{
			board[p.y][p.x] = 0;
			return -1;
		}
	getScore();
	turn = (turn == 'b' ? 'w' : 'b');
	return 1;
}

int Game::aiMove()
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	alpha = INT_MIN;
	beta = INT_MAX;
	Game g = *this;
	Selector::minimax(g, 0, turn, false);
	std::chrono::steady_clock::time_point
	end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " <<
	std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
	/ 1000.0f << std::endl;
	return move(Selector::nxMove);
}
