#include "Display.hpp"
#include "Selector.hpp"

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
	if (game.won)
		cout << (game.won == 'b' ? "Black" : "White") << " had won the game\n";
}

Display::Display() {}

Display::Display(Game g) : game(g)
{
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

Display::~Display()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(rend);
}

void Display::outputMove()
{
	cout << "Free 4 : " << game.comp[0] << ", " << game.comp[1] << endl;
	cout << "Half-open 4 : " << game.comp[2] << ", " << game.comp[3] << endl;
	cout << "Free 3 : " << game.comp[4] << ", " << game.comp[5] << endl;
	cout << "Half-open 3 / Free 2 : " << game.comp[6] << ", " << game.comp[7] << endl;
	cout << "Capture : " << game.cap_b << ", " << game.cap_w << endl;
	cout << "Player : " << game.score << endl;
	refresh();
}

int		Display::run()
{
	pos p;
	if (game.ai == 'b')
	{
		game.board[9][9] = 'b';
		game.turn = 'w';
	}
	refresh();
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_ESCAPE))
				exit (1);
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				p.x = event.motion.x * 19.0 / float(W);
				p.y = event.motion.y * 19.0 / float(H);
				hist.push(game);
				while (forward.size())
					forward.pop();
				if (game.move(p) == -1)
				{
					cout << "Invalid move\n";
					hist.pop();
				}
				outputMove();
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT && hist.size())
			{
				forward.push(game);
				game = hist.top();
				hist.pop();
				while (hist.size() && game.turn == game.ai)
				{
					forward.push(game);
					game = hist.top();
					hist.pop();
				}
				refresh();
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT && forward.size())
			{
				hist.push(game);
				game = forward.top();
				forward.pop();
				while (forward.size() && game.turn == game.ai)
				{
					hist.push(game);
					game = forward.top();
					forward.pop();
				}
				refresh();
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_h)
			{
				hist.push(game);
				game.ai = game.turn;
				if (game.aiMove() == -1)
				{
					cout << "Invalid move\n";
					hist.pop();
				}
				refresh();
				usleep(500000);
				game = hist.top();
				hist.pop();
				refresh();
			}
		}
		if (game.ai == game.turn)
		{
			hist.push(game);
			if (game.aiMove() == -1)
			{
				cout << "Invalid move\n";
				hist.pop();
			}
			outputMove();
		}
	}
}
