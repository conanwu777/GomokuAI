#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

int main (int ac, char **av)
{
	char ai;

	srand(time(NULL));
	if (ac == 1)
		ai = 0;
	else if (ac == 2 && !strcmp(av[1], "-w"))
		ai = 'b';
	else if (ac == 2 && !strcmp(av[1], "-b"))
		ai = 'w';
	else
	{
		cout << "Usage: Gomoku [-w | -b]" << endl;
		exit(1);
	}
    Game game;
    game.ai = ai;
    Display disp(game);
    disp.run();
    return 0;
}