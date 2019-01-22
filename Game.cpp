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

bool Game::checkLineThrees(deque<char> &line, char target){
	bool leftOpen = false;
	int count = 0;
	bool hole = false;
	int num = 0;
	for (auto i = line.begin(); i != line.end() ; i++){
		if (*i == 0){
			if (!leftOpen){
				leftOpen = true;
			}
			else if (count == 3){
				return true;
			}
			else if (hole == false){
				hole = true;
			}
			else{
				return 0;
			}
		}
		else if (*i == target){
			if (leftOpen){
				count++;
			}
		}
		else{
			if (leftOpen)
				return 0;
		}
	}
	return 0;
}

int Game::checkThree(int x, int y, int xOff, int yOff)
{
	bool emptyLeft = false;
	bool emptyRight = false;
	
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

	if (x < 0 || x > 18 || y < 0 || y > 18)
		return -1;
	if (board[y][x])
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
	if (won)
		cout << won << " has won the game!\n";
	turn = (turn == 'b' ? 'w' : 'b');
	return 1;
}

int Game::aiMove()
{
	int x = -1;
	int y = -1;
	while (!inBound(x, y) || board[x][y])
	{
		x = rand() % 19;
		y = rand() % 19;
	}
	return move(x, y);
}
