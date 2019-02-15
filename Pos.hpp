#ifndef POS_HPP
# define POS_HPP

# include <iostream>
using namespace std;

struct pos
{
	int x;
	int y;
	pos operator=(const pos& p);
	pos(const pos &p);
	pos(){
		x = 0;
		y = 0;
	}
	pos(int nx, int ny)
	{
		x = nx;
		y = ny;
	}
};

ostream &operator<<(ostream &os, const pos& p);

struct Hash
{
    size_t operator()(pos const& p) const noexcept
    {
        return p.x * 20 + p.y;
    }
};

#endif