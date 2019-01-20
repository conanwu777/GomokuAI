#include "gomoku.hpp"
#include "Game.hpp"
#include "Display.hpp"

int main ()
{
    Game game;
    Display disp(game);

    disp.run();
    return 0;
}