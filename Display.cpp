#include "Display.hpp"

void	Display::refresh()
{
	SDL_RenderCopy(rend, background, NULL, NULL);
	SDL_Rect rect;
	rect.w = W / 19;
	rect.h = H / 19;
	for (int i = 0; i < 19; i++)
	{
		rect.y = i * rect.h;
		for (int j = 0; j < 19; j++)
		{
			rect.x = j * rect.w;
			if (game.board[i][j] == 'b')
				SDL_RenderCopy(rend, black, NULL, &rect);
			else if (game.board[i][j] == 'w')
				SDL_RenderCopy(rend, white, NULL, &rect);
		}
	}
	SDL_RenderPresent(rend);
}

void	Display::get_input()
{
	while (SDL_PollEvent(&event))
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
			exit (1);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			int x = event.motion.x * 19.0 / float(W);
			int y = event.motion.y * 19.0 / float(H);
			if (game.move(x, y) == -1)
				cout << "Invalid move\n";
		}
}

Display::Display() {}

Display::Display(Game g) : game(g) {
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Gomoku", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, 0);
	SDL_Surface *tmpSurf;
	tmpSurf = SDL_LoadBMP("tex/background.bmp");
	background = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/white.bmp");
	white = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/black.bmp");
	black = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
}

Display::~Display(){
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(rend);
}

int		Display::run()
{
	if (game.ai == 'b')
	{
		game.board[9][9] = 'b';
		game.turn = 'w';
	}
	while (1)
	{
		get_input();
		refresh();
		if (game.ai == game.turn)
		{
			game.aiMove();
			refresh();
		}
	}
}
