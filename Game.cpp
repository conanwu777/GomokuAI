#include "Game.hpp"
#include "Selector.hpp"
#include <chrono>

bool Game::inBound(int x, int y) const
{
	return (x >= 0 && y >= 0 && x < 19 && y < 19);
}

bool Game::adjacent(int x, int y) const
{
	if (inBound(x - 1, y) && board[y][x - 1])
		return true;
	if (inBound(x, y - 1) && board[y - 1][x])
		return true;
	if (inBound(x + 1, y) && board[y][x + 1])
		return true;
	if (inBound(x, y + 1) && board[y + 1][x])
		return true;
	if (inBound(x - 1, y - 1) && board[y - 1][x - 1])
		return true;
	if (inBound(x + 1, y + 1) && board[y + 1][x + 1])
		return true;
	if (inBound(x + 1, y - 1) && board[y - 1][x + 1])
		return true;
	if (inBound(x - 1, y + 1) && board[y + 1][x - 1])
		return true;
	return false;
}

Game::Game() : turn('b'), won(0), cap_b(0), cap_w(0),
alpha(INT_MIN), beta(INT_MAX)
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
	cap_b = g.cap_b;
	cap_w = g.cap_w;
	for (int x = 0; x < 19; x++)
		for (int y = 0; y < 19; y++)
			board[y][x] = g.board[y][x];
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
bool Game::capture(int x, int y, int dx, int dy, char opp)
{
	if (inBound(x + 3 * dx, y + 3 * dy)
		&& board[y + dy][x + dx] == opp
		&& board[y + 2 * dy][x + 2 * dx] == opp
		&& board[y + 3 * dy][x + 3 * dx] == turn)
	{
		board[y + dy][x + dx] = 0;
		board[y + 2 * dy][x + 2 * dx] = 0;
		return true;
	}
	return false;
}

bool Game::checkCapture(int x, int y)
{
	int c = 0;
	char opp = (turn == 'b' ? 'w' : 'b');
	if (capture(x, y, 1, 0, opp))
		c++;
	if (capture(x, y, 0, 1, opp))
		c++;
	if (capture(x, y, -1, 0, opp))
		c++;
	if (capture(x, y, 0, -1, opp))
		c++;
	if (capture(x, y, 1, 1, opp))
		c++;
	if (capture(x, y, 1, -1, opp))
		c++;
	if (capture(x, y, -1, 1, opp))
		c++;
	if (capture(x, y, -1, -1, opp))
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

int Game::checkThree(int x, int y, int xOff, int yOff)
{
	deque<char> cur;

	for (int i = -4; i < 5; i++)
	{
		if (cur.size() >= 6)
			cur.pop_front();
		if (x + i * xOff >= 0 && x + i * xOff < 19
			&& y + i * yOff >= 0 && y + i * yOff < 19)
			cur.push_back(board[y + i * yOff][x + i * xOff]);
		if (checkLineThrees(cur, turn))
			return 1;
	}
	return 0;
}

bool Game::checkValid(int x, int y)
{
	int count = 0;
	count += checkThree(x, y, 1, 0);
	count += checkThree(x, y, 0, 1);
	count += checkThree(x, y, 1, 1);
	count += checkThree(x, y, 1, -1);
	if (count >= 2){
		return false;
	}
	return true;
}

int Game::move(int x, int y)
{
	bool capture = true;

	if (!inBound(x, y) || board[y][x])
		return -1;
	board[y][x] = turn;
	won = checkWin();
	if (!won)
		capture = checkCapture(x, y);
	if (!capture)
		if (!checkValid(x, y))
		{
			board[y][x] = 0;
			return -1;
		}
	
	turn = (turn == 'b' ? 'w' : 'b');
	return 1;
}


int Game::aiMove()
{
	int x, y;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	alpha = INT_MIN;
	beta = INT_MAX;
	Selector::minimax(*this, 0, x, y, turn);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0f<<std::endl;
	return move(x, y);
}
