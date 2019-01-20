#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "gomoku.hpp"
# include "Game.hpp"
# include <SDL2/SDL.h>

# define W 1200
# define H 1200

class Display {
private:
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Event event;
    SDL_Texture* background;
    SDL_Texture* white;
    SDL_Texture* black;
    Game game;
    Display();

public:
    Display(Game g);
    ~Display();
    void refresh();
    void SDL_Start();
    void get_input();
    int run();
};

#endif
