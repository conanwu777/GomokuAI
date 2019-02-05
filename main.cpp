#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

bool b;
bool w;
float mult_b = 1;
float mult_w = 1;

int main (int ac, char **av)
{
	b = false;
	w = false;

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
    Game *game = new Game();
    Display disp(game);
    disp.run();
    return 0;
}