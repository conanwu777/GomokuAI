#include "Display.hpp"

// void	Display::ponies()
// {
// 	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
// 	SDL_Rect rect;
// 	rect.w = (W - 2 * M) * 2 / (N - 1) / 3;
// 	rect.h = (W - 2 * M) * 2 / (N - 1) / 3;
// 	rect.x = 50;
// 	rect.y = 50;
// 	SDL_RenderCopy(rend, p1, NULL, &rect);
// }

// void	Display::connect(Event const & e, Event const & p)
// {
// 	int y = (runTime - e.getData().timestamp) * GAP;
// 	int x = M + e.getData().owner * (W - 2 * M) / (N - 1);
// 	int y2 = (runTime - p.getData().timestamp) * GAP;
// 	int x2 = M + p.getData().owner * (W - 2 * M) / (N - 1);
// 	if (y < H || y2 < H){
// 		SDL_RenderDrawLine(rend, x, y, x2, y2);
// 		SDL_RenderDrawLine(rend, x, y - 1, x2, y2 - 1);
// 		SDL_RenderDrawLine(rend, x - 1, y, x2 - 1, y2);
// 	}
// }

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
	while (1)
	{
		get_input();
		refresh();
	}
}
