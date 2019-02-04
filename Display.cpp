#include "Display.hpp"

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
	rect.y = 575;
	SDL_RenderCopy(rend, right, NULL, &rect);
	rect.w = 50;
	rect.h = 50;
	for (int i = 0; i < 19; i++)
	{
		rect.y = i * rect.h + 25;
		for (int j = 0; j < 19; j++)
		{
			rect.x = j * rect.w + 325;
			if (game->board[i][j] == 'b')
				SDL_RenderCopy(rend, black, NULL, &rect);
			else if (game->board[i][j] == 'w')
				SDL_RenderCopy(rend, white, NULL, &rect);
		}
	}

	capturedPieces();
	printNumber(blackTime.min, 84, 335, 0, 0.7);
	printNumber(blackTime.sec, 134, 335, 0, 0.7);
	printNumber(blackTime.milli, 185, 335, 0, 0.7);

	printNumber(whiteTime.min, 1384, 910, 1, 0.7);
	printNumber(whiteTime.sec, 1434, 910, 1, 0.7);
	printNumber(whiteTime.milli, 1485, 910, 1, 0.7);

	printNumber(blackTotalTime.min, 55, 275, 0, 1);
	printNumber(blackTotalTime.sec, 126, 275, 0, 1);
	printNumber(blackTotalTime.milli, 197, 275, 0, 1);

	printNumber(whiteTotalTime.min, 1355, 850, 1, 1);
	printNumber(whiteTotalTime.sec, 1426, 850, 1, 1);
	printNumber(whiteTotalTime.milli, 1497, 850, 1, 1);

	if (game->trueWon){
		SDL_Rect box = {550, 100, 500, 98};
		SDL_RenderCopy(rend, (game->trueWon == 'w' ? whiteWin : blackWin), NULL, &box);
	}
	SDL_RenderPresent(rend);
}

void Display::updateTime(char color){
	float time = std::chrono::duration_cast<std::chrono::milliseconds>
	(end - begin).count() / 1000.0;
	if (color == 'b')
	{
		blackTime.min = (int)time / 60;
		blackTime.sec = (int)time % 60;
		blackTime.milli = (time - (int)time) * 100;
		blackTotalTime.min += blackTime.min;
		blackTotalTime.sec += blackTime.sec;
		blackTotalTime.milli += blackTime.milli;

		if (blackTotalTime.milli >= 100)
		{
			blackTotalTime.sec++;
			blackTotalTime.milli -= 100;
		}
		if (blackTotalTime.sec >= 60)
		{
			blackTotalTime.min++;
			blackTotalTime.sec -= 60;
		}
	}
	else
	{
		whiteTime.min = (int)time / 60;
		whiteTime.sec = (int)time % 60;
		whiteTime.milli = (time - (int)time) * 100;
		whiteTotalTime.min += whiteTime.min;
		whiteTotalTime.sec += whiteTime.sec;
		whiteTotalTime.milli += whiteTime.milli;

		if(whiteTotalTime.milli >= 100){
			whiteTotalTime.sec++;
			whiteTotalTime.milli -= 100;
		}
		if(whiteTotalTime.sec >= 60){
			whiteTotalTime.min++;
			whiteTotalTime.sec -= 60;
		}
	}
}

void Display::printNumber(int num, int x, int y, bool b, float scale){
	while(num >= 100){
		num /= 10;
	}
	SDL_Rect rect = {x, y, (int)(24 * scale), (int)(40 * scale)};
	SDL_Rect numBox = {0, 0, 30, 50};
	numBox.x = 30 * (num / 10);
	SDL_RenderCopy(rend, (b ? num_w : num_b), &numBox, &rect);
	rect.x += (int)(24 * scale);
	numBox.x = 30 * (num % 10);
	SDL_RenderCopy(rend, (b ? num_w : num_b), &numBox, &rect);
}

Display::Display() {}

void    Display::capturedPieces()
{
    SDL_Rect rect;

    rect.w = 50;
    rect.h = 50;
    rect.y = H - 100;
    for (int i = 0; i < game->cap_b; i++)
    {
        rect.x = 175;
        SDL_RenderCopy(rend, white, NULL, &rect);
        rect.x += 50;
        rect.y -= 25;
        SDL_RenderCopy(rend, white, NULL, &rect);
        rect.y -= 50;
    }
    rect.y = 50;
    for (int i = 0; i < game->cap_w; i++)
    {
        rect.x = 1375;
        SDL_RenderCopy(rend, black, NULL, &rect);
        rect.x -= 50;
        rect.y += 25;
        SDL_RenderCopy(rend, black, NULL, &rect);
        rect.y += 50;
    }
}

Display::Display(Game *g) : game(g)
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
	tmpSurf = SDL_LoadBMP(b ? "tex/ai_b.bmp" : "tex/man_b.bmp");
	left = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP(w ? "tex/ai_w.bmp" : "tex/man_w.bmp");
	right = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/nums_b.bmp");
	num_b = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/nums_w.bmp");
	num_w = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/whiteWin.bmp");
	whiteWin = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	tmpSurf = SDL_LoadBMP("tex/blackWin.bmp");
	blackWin = SDL_CreateTextureFromSurface(rend, tmpSurf);
	SDL_FreeSurface(tmpSurf);
	whiteTime.milli = 0;
	whiteTime.sec = 0;
	whiteTime.min = 0;

	blackTime.milli = 0;
	blackTime.sec = 0;
	blackTime.min = 0;

	whiteTotalTime.milli = 0;
	whiteTotalTime.sec = 0;
	whiteTotalTime.min = 0;

	blackTotalTime.milli = 0;
	blackTotalTime.sec = 0;
	blackTotalTime.min = 0;
	begin = std::chrono::steady_clock::now();
}

Display::~Display()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(rend);
}

void Display::outputMove()
{
	if (game->pv)
	{
		cout << game->pv->nxs.size() << endl;
		for (int i = 0; i < game->pv->moves.size(); i++)
		{
			cout << game->pv->nxs[game->pv->moves[i]]->score << " : "
			<< game->pv->moves[i].x << ", " << game->pv->moves[i].y << endl;
		}
	}
	refresh();

}

void		Display::checkClick()
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		pos p;
		p.x = (event.motion.x - 325) / 50.0;
		p.y = (event.motion.y - 25) / 50.0;
		Game *nxGame = game->move(p);
		if (!nxGame)
		{
			cout << "Invalid move\n";
			return ;
		}
		while (forward.size())
			forward.pop();
		end = std::chrono::steady_clock::now();
		updateTime(game->turn);
		game = nxGame;
		outputMove();
		begin = std::chrono::steady_clock::now();

		if (game->trueWon)
			winTrigger();
	}
}

bool		Display::isAITurn()
{
	if (game->turn == 'b' && b)
		return true;
	if (game->turn == 'w' && w)
		return true;
	return false;
}

void		Display::checkHist()
{
	if (event.type != SDL_KEYUP)
		return ;
	if (event.key.keysym.sym == SDLK_LEFT && game->pv)
	{
		forward.push(game);
		game = game->pv;
		while (game->pv && isAITurn())
		{
			forward.push(game);
			game = game->pv;
		}
		refresh();
	}
	if (event.key.keysym.sym == SDLK_RIGHT && forward.size())
	{
		game = forward.top();
		forward.pop();
		while (forward.size() && isAITurn())
		{
			game = forward.top();
			forward.pop();
		}
		refresh();
	}
}

void	Display::checkHint()
{
	if (event.type != SDL_KEYUP || event.key.keysym.sym != SDLK_h)
		return ;
	bool tb = b;
	bool tw = w;
	b = true;
	w = true;
	Game *nextGame = game->aiMove();
	if (nextGame == NULL)
		cout << "Invalid move\n";
	else
		game = nextGame;
	refresh();
	usleep(500000);
	game = game->pv;
	refresh();
	b = tb;
	w = tw;
}

void	Display::winTrigger(){
	takeInput = false;
	refresh();
}

void		Display::run()
{
	if (b)
	{
		game->board[9][9] = 'b';
		game->turn = 'w';
	}
	refresh();
	game->rankMoves();
	while (1)
	{
		if (!isAITurn() && SDL_PollEvent(&event))
		{
//this should be in a separate thread
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_ESCAPE))
				exit (1);
			if (takeInput)
				checkClick();
			checkHist();
			checkHint();
		}
		if (game->trueWon)
			winTrigger();
		else
			takeInput = true;
		if (takeInput && isAITurn())
		{
			begin = std::chrono::steady_clock::now();
			Game *nextGame = game->aiMove();
			game = nextGame;
			outputMove();
			end = std::chrono::steady_clock::now();
			updateTime(game->turn);
cout << endl << endl;
		}
	}
}
