#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "gomoku.hpp"
# include "Game.hpp"
# include <stack>
# include <SDL2/SDL.h>

# define W 1600
# define H 1000

class Display {
private:
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Event event;
	SDL_Texture* background;
	SDL_Texture* white;
	SDL_Texture* black;
	SDL_Texture* manIcon;
	SDL_Texture* aiIcon;
	Game game;
	Display();

public:
	stack<Game> hist;
	stack<Game> forward;
	Display(Game g);
	~Display();
	void	refresh();
	int		run();
	void	outputMove();
	void	checkClick();
	void	checkHist();
	void	checkHint();
	void	checkAIMove();
};

#endif
