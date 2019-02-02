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
	SDL_RenderCopy(rend, left, NULL, &rect);
	rect.x = 1300;
	rect.y = 0;
	SDL_RenderCopy(rend, right, NULL, &rect);
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
	printNumber(whiteTime.min, 10, 300, 1);
	printNumber(whiteTime.sec, 100, 300, 1);
	printNumber(whiteTime.milli, 200, 300, 1);

	printNumber(blackTime.min, 1300, 300, 0);
	printNumber(blackTime.sec, 1400, 300, 0);
	printNumber(blackTime.milli, 1500, 300, 0);

	SDL_RenderPresent(rend);
	if (game.won)
		cout << (game.won == 'b' ? "Black" : "White") << " had won the game\n";
}

void Display::updateTime(char color){
	float time = std::chrono::duration_cast<std::chrono::milliseconds>
	(end - begin).count() / 1000.0;
	if (color == 'b')
	{
		blackTime.min = (int)time / 60;
		blackTime.sec = (int)time % 60;
		blackTime.milli = (time - (int)time) * 100;
	}
	else
	{
		whiteTime.min = (int)time / 60;
		whiteTime.sec = (int)time % 60;
		whiteTime.milli = (time - (int)time) * 100;
	}
}

void Display::printNumber(int num, int x, int y, bool b){
	while(num >= 100){
		num /= 10;
	}
	SDL_Rect rect = {x, y, 50, 50};
	SDL_Rect numBox = {0, 0, 50, 50};
	numBox.x = 50 * (num / 10);
	SDL_RenderCopy(rend, (b ? num_w : num_b), &numBox, &rect);
	rect.x += 50;
	numBox.x = 50 * (num % 10);
	SDL_RenderCopy(rend, (b ? num_w : num_b), &numBox, &rect);
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
	tmpSurf = SDL_LoadBMP(game.b ? "tex/ai_w.bmp" : "tex/man_w.bmp");
	left = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP(game.w ? "tex/ai_b.bmp" : "tex/man_b.bmp");
	right = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/nums_b.bmp");
	num_b = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/nums_w.bmp");
	num_w = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	whiteTime.milli = 0;
	whiteTime.sec = 0;
	whiteTime.min = 0;

	blackTime.milli = 0;
	blackTime.sec = 0;
	blackTime.min = 0;
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

bool		Display::isAITurn()
{
	if (game.turn == 'b' && game.b)
		return true;
	if (game.turn == 'w' && game.w)
		return true;
	return false;
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
		while (hist.size() && isAITurn())
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
		while (forward.size() && isAITurn())
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
		game.b = true;
		game.w = true;
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
	hist.push(game);
	if (game.aiMove() == -1)
	{
		cout << "Invalid move\n";
		hist.pop();
	}
	outputMove();
}

int		Display::run()
{
	if (game.b)
	{
		game.board[9][9] = 'b';
		game.turn = 'w';
	}
	refresh();
	while (1)
	{
		begin = std::chrono::steady_clock::now();
		while (!isAITurn() && SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_ESCAPE))
				exit (1);
			checkClick();
			checkHist();
			checkHint();
		}
		end = std::chrono::steady_clock::now();
		updateTime(game.turn);
		if (isAITurn())
		{
			begin = std::chrono::steady_clock::now();
			checkAIMove();
			end = std::chrono::steady_clock::now();
			updateTime(game.turn);
		}
	}
}
