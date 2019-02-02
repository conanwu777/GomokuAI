#include "Display.hpp"
#include "Selector.hpp"

void	Display::refresh()
{
	SDL_Rect rect;
	SDL_RenderCopy(rend, background, NULL, NULL);
	rect.w = 300;
	rect.h = 300;
	rect.x = 0;
	rect.y = 0;
	SDL_RenderCopy(rend, game.ai == 'b' ? aiIcon : manIcon, NULL, &rect);
	rect.x = 1300;
	rect.y = 0;
	SDL_RenderCopy(rend, game.ai == 'b' ? manIcon : aiIcon, NULL, &rect);
	rect.w = 50;
	rect.h = 50;
	for (int i = 0; i < 19; i++)
	{
		rect.y = i * rect.h + 25;
		for (int j = 0; j < 19; j++)
		{
			rect.x = j * rect.w + 325;
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
	tmpSurf = SDL_LoadBMP("tex/board.bmp");
	background = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/white.bmp");
	white = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/black.bmp");
	black = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP(game.ai == 'b' ? "tex/man_b.bmp" : "tex/man_w.bmp");
	manIcon = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	if (!game.ai)
		tmpSurf = SDL_LoadBMP("tex/man_b.bmp");
	else
		tmpSurf = SDL_LoadBMP(game.ai == 'b' ? "tex/ai_w.bmp" : "tex/ai_b.bmp");
	aiIcon = SDL_CreateTextureFromSurface(rend, tmpSurf);
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

void		Display::checkClick()
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		pos p;
		p.x = (event.motion.x - 325) / 50.0;
		p.y = (event.motion.y - 25) / 50.0;
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
}

void		Display::checkHist()
{
	if (event.type != SDL_KEYUP)
		return ;
	if (event.key.keysym.sym == SDLK_LEFT && hist.size())
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
	if (event.key.keysym.sym == SDLK_RIGHT && forward.size())
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
}

void	Display::checkHint()
{
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

void	Display::checkAIMove()
{
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
			checkClick();
			checkHist();
			checkHint();
		}
		checkAIMove();
	}
}
