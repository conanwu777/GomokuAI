#include "Pos.hpp"

bool operator==(const pos& lhs, const pos& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

pos operator*(int num, const pos& p)
{
	pos nPos;
	nPos.x = p.x * num;
	nPos.y = p.y * num;
	return nPos;
}

pos operator+(const pos& p, const pos& other)
{
	pos nPos;
	nPos.x = p.x + other.x;
	nPos.y = p.y + other.y;
	return nPos;
}

ostream &operator<<(ostream &os, const pos& p)
{
	os << "(" << p.x << ", " << p.y << ")";
	return os;
}

pos pos::operator=(const pos& p)
{
	x = p.x;
	y = p.y;
	return *this;
}

pos::pos(const pos &p)
{
	x = p.x;
	y = p.y;
}
