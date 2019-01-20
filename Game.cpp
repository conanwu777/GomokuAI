#include "Game.hpp"

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

void Game::checkCapture(int x, int y)
{
	int c = 0;
	char opp = (turn == 'b' ? 'w' : 'b');
	if (x >= 3 && board[x - 1][y] == opp
		&& board[x - 2][y] == opp && board[x - 3][y] == turn)
	{
		board[x - 1][y] = 0;
		board[x - 2][y] = 0;
		c++;
	}
	if (x <= 15 && board[x + 1][y] == opp
		&& board[x + 2][y] == opp && board[x + 3][y] == turn)
	{
		board[x + 1][y] = 0;
		board[x + 2][y] = 0;
		c++;
	}
	if (y >= 3 && board[x][y - 1] == opp
		&& board[x][y - 2] == opp && board[x][y - 3] == turn)
	{
		board[x][y - 1] = 0;
		board[x][y - 2] = 0;
		c++;
	}
	if (y <= 15 && board[x][y + 2] == opp
		&& board[x][y + 2] == opp && board[x][y + 3] == turn)
	{
		board[x][y + 1] = 0;
		board[x][y + 2] = 0;
		c++;
	}
	if (x >= 3 && y >= 3 && board[x - 1][y - 1] == opp
		&& board[x - 2][y - 2] == opp && board[x - 3][y - 3] == turn)
	{
		board[x - 1][y - 1] = 0;
		board[x - 2][y - 2] = 0;
		c++;
	}
	if (x >= 3 && y <= 15 && board[x - 1][y + 1] == opp
		&& board[x - 2][y + 2] == opp && board[x - 3][y + 3] == turn)
	{
		board[x - 1][y + 1] = 0;
		board[x - 2][y + 2] = 0;
		c++;
	}
	if (x <= 15 && y >= 3 && board[x + 1][y - 1] == opp
		&& board[x + 2][y - 2] == opp && board[x + 3][y - 3] == turn)
	{
		board[x + 1][y - 1] = 0;
		board[x + 2][y - 2] = 0;
		c++;
	}
	if (x <= 15 && y <= 15 && board[x + 1][y + 1] == opp
		&& board[x + 2][y + 2] == opp && board[x + 3][y + 3] == turn)
	{
		board[x + 1][y + 1] = 0;
		board[x + 2][y + 2] = 0;
		c++;
	}
	(turn == 'b' ? cap_b : cap_w) += 2 * c;
	if ((turn == 'b' ? cap_b : cap_w) >= 10)
		won = turn;
}

int Game::move(int x, int y)
{
	if (x < 0 || x > 18 || y < 0 || y > 18)
		return -1;
	if (board[y][x])
		return -1;
	board[y][x] = turn;
	cout << x << ", " << y << endl;
	won = checkWin();
	if (!won)
		checkCapture(y, x);
	if (won)
		cout << won << " has won the game!\n";
	turn = (turn == 'b' ? 'w' : 'b');
	return 1;
}
