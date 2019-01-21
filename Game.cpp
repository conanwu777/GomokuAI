#include "Game.hpp"

bool Game::inBound(int x, int y)
{
	return (x >= 0 && y >= 0 && x < 19 && y < 19);
}

Game::Game() : turn('b'), won(0), cap_b(0), cap_w(0)
{
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			board[i][j] = 0;
}

Game::~Game() {}

char Game::checkWin()
{
	char ret = 0;
	for (int i = 0; i < 19; i++)
	{
		int c = 0;
		int r = 0;
		for (int j = 0; j < 19; j++)
		{
			if (board[i][j] && j > 0 && board[i][j] == board[i][j - 1])
				c++;
			else if (board[i][j])
			{
				c = 1;
				ret = board[i][j];
			}
			else
				c = 0;

			if (board[j][i] && j > 0 && board[j][i] == board[j - 1][i])
				r++;
			else if (board[j][i])
			{
				r = 1;
				ret = board[i][j];
			}
			else
				r = 0;
			if (r >= 5 || c >= 5)
				return ret;
		}
	}
	for (int sum = 4; sum <= 32; sum++)
	{
		int c = 0;
		for (int j = max(0, sum - 18); j <= min(18, sum); j++)
		{
			int i = sum - j;
			if (board[i][j] && j > max(0, sum - 18) && board[i + 1][j - 1] == board[i][j])
				c++;
			else if (board[i][j])
			{
				c = 1;
				ret = board[i][j];
			}
			else
				c = 0;
			if (c >= 5)
				return ret;
		}
	}
	for (int diff = -14; diff <= 14; diff++)
	{
		int c = 0;
		for (int j = max(0, diff); j <= min(18, 18 + diff); j++)
		{
			int i = j - diff;
			if (board[i][j] && j > max(0, diff) && board[i - 1][j - 1] == board[i][j])
				c++;
			else if (board[i][j])
			{
				c = 1;
				ret = board[i][j];
			}
			else
				c = 0;
			if (c >= 5)
				return ret;
		}
	}
	return 0;
}

bool Game::capture(int x, int y, int dx, int dy, char opp)
{
	if (inBound(x + 3 * dx, y * 3 * dy)
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

void Game::checkCapture(int x, int y)
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
}

bool Game::checkThree(int x, int y, int dx, int dy)
{
	int stx = x;
	int sty = y;
	for (int i = 0; i <= 4 && inBound(stx, sty); i++)
	{
		stx -= dx;
		sty -= dy;
	}
	for (int i = 0; i <= 8 && inBound(stx, sty); i++)
	{
		if (!board[sty][stx])
		{
			int pieces = 0;
			bool gap = false;
			for (int j = 1; j <= 4 && inBound(stx + j * dx, sty + j * dy); j++)
			{
				if (board[sty + j * dy][stx + j * dx] == turn)
					pieces++;
				else if (!board[sty + j * dy][stx + j * dx])
				{
					if (pieces == 3)
						return true;
					if (gap)
						break ;
					gap = true;
				}
				else
					break ;
			}
			stx += dx;
			sty += dy;
		}
	}
	return false;
}

bool Game::doubleThree(int x, int y)
{
	int c = 0;

	if (checkThree(x, y, 1, 0))
		c++;
	if (checkThree(x, y, 0, 1))
		c++;
	if (checkThree(x, y, 1, 1))
		c++;
	if (checkThree(x, y, 1, -1))
		c++;
	if (c >= 2)
		return true;
	return false;

}

int Game::move(int x, int y)
{
	if (x < 0 || x > 18 || y < 0 || y > 18)
		return -1;
	if (board[y][x])
		return -1;
	if (doubleThree(x, y))
		return -1;
	board[y][x] = turn;
	cout << x << ", " << y << endl;
	won = checkWin();
	if (!won)
		checkCapture(x, y);
	if (won)
		cout << won << " has won the game!\n";
	turn = (turn == 'b' ? 'w' : 'b');
	return 1;
}
