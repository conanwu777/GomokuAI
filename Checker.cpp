#include "gomoku.hpp"
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
			else if (curBlock == 2 && open){
				comp[(curPlayer == 'b' ? 8 : 9)]++;
			}
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
			else if (curBlock == 2 && !open){
				comp[(curPlayer == 'b' ? 8 : 9)]++;
			}
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
	if ((open && curBlock == 4) || (prevBlock && curBlock && curBlock + prevBlock == 4 && curPlayer == prevPlayer)){
		comp[(curPlayer == 'b' ? 2 : 3)]++;
	}
	if (open && curBlock == 3){
		comp[(curPlayer == 'b' ? 6 : 7)]++;
	}
	return 0;
}

char Game::checkWin()
{
	char ret = 0;
	bzero(&comp[0], sizeof(int) * 10);
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
