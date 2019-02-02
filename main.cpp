#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

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

int main (int ac, char **av)
{
	bool b = false;
	bool w = false;

	srand(time(NULL));
	if (ac == 1)
		;
	else if (ac == 2 && !strcmp(av[1], "-w"))
		b = true;
	else if (ac == 2 && !strcmp(av[1], "-b"))
		w = true;
	else if (ac == 2 && !strcmp(av[1], "-a"))
	{
		b = true;
		w = true;
	}
	else
	{
		cout << "Usage: Gomoku [-w | -b | -a]" << endl;
		exit(1);
	}
    Game game(b, w);
    Display disp(game);
    disp.run();
    return 0;
}