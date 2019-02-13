#include "Display.hpp"
#include "Selector.hpp"

pos threadWorking;
mutex mtx;

void	Display::printTime()
{
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
}

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
	printTime();
	if (game->trueWon)
	{
		SDL_Rect box = {550, 100, 500, 98};
		SDL_RenderCopy(rend,
			(game->trueWon == 'w' ? whiteWin : blackWin), NULL, &box);
	}
	SDL_RenderPresent(rend);
}

void Display::addTime(timeFrame *frame, float time)
{
	frame->min += (int)time / 60;
	frame->sec += (int)time % 60;
	frame->milli += (time - (int)time) * 100;

	if (frame->milli >= 100)
	{
		frame->sec++;
		frame->milli -= 100;
	}
	if (frame->sec >= 60)
	{
		frame->min++;
		frame->sec -= 60;
	}
}

void Display::updateTime(char color, float time)
{
	timeFrame *frame;
	timeFrame *totalFrame;

	frame = (color == 'b' ? &blackTime : &whiteTime);
	totalFrame = (color == 'b' ?
		&blackTotalTime : &whiteTotalTime);

	addTime(frame, time);
	addTime(totalFrame, time);
}

void Display::printNumber(int num, int x, int y, bool b, float scale)
{
	while(num >= 100)
		num /= 10;
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

Display::Display(Game *g) : game(g), AIPlayed(false)
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

	bzero(&whiteTime, sizeof(whiteTime));
	bzero(&blackTime, sizeof(blackTime));
	bzero(&whiteTotalTime, sizeof(whiteTotalTime));
	bzero(&blackTotalTime, sizeof(blackTotalTime));
}

Display::~Display()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(rend);
}

bool	isCapture(Game *g)
{
	if (!g->pv)
		return false;
	if (g->cap_b > g->pv->cap_b || g->cap_w > g->pv->cap_w)
		return true;
	return false;
}

int		Display::TimerThread(void* param)
{
	Display *disp = (Display*) param;
	while (1)
	{
		SDL_Delay(50);
		if (!disp->game->trueWon)
			disp->updateTime(disp->game->turn, 0.05);
		disp->refresh();
	}
}

int		Display::preComp(void* param)
{
	Display *disp = (Display*) param;
	int d = 4;
	while (1)
	{
		while (disp->game->trueWon)
			;
		if (guessMv.size())
			continue ;
		char c = disp->game->turn == 'b' ? 'w' : 'b';
		disp->game->rankMoves();
		d = 4;
		disp->AIPlayed = false;
		while (d < 361)
		{
cout << "depth : " << d << endl;
			for (int i = 0; i < disp->game->moves.size() && i < CUTOFF; i++)
			{
cout << disp->game->moves[i].x << " , " << disp->game->moves[i].y << endl;
				Game *ptm = disp->game->move(disp->game->moves[i]);
				Selector sele(ptm, ptm->turn, d);
				threadWorking = disp->game->moves[i];
				mtx.lock();
cout << "thread\n";
				sele.minimax(0, false);
				mtx.unlock();
				if (disp->AIPlayed || disp->game->trueWon)
					break;
				guessMv[disp->game->moves[i]] = sele.out;
			}
			threadWorking = {-1, -1};
			if (disp->AIPlayed || disp->game->trueWon)
			{
				guessMv.clear();
				break;
			}
			d += 2;
		}
		while (!disp->AIPlayed && !disp->game->trueWon)
			;
	}
}

void	Display::checkClick()
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
		game->rankMoves();

		if (game->moves.size() && isCapture(game->nxs[game->moves[0]]))
			(game->turn == 'b' ? mult_b : mult_w) *= 0.9;
		if (game->moves.size() && isCapture(nxGame))
			(game->turn == 'b' ? mult_b : mult_w) /= 0.9;
// cout << "mult : " << mult_b << ", " << mult_w << endl;
		game->freeGames(p, true);
		game = nxGame;

		if (game->turn == 'w')
			bzero(&whiteTime, sizeof(whiteTime));
		else
			bzero(&blackTime, sizeof(blackTime));

		if (game->trueWon)
			takeInput = false;
	}
}

bool	Display::isAITurn()
{
	if (game->turn == 'b' && b)
		return true;
	if (game->turn == 'w' && w)
		return true;
	return false;
}

void	Display::checkHist()
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
		bzero(&whiteTime, sizeof(whiteTime));
		bzero(&blackTime, sizeof(blackTime));
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
		bzero(&whiteTime, sizeof(whiteTime));
		bzero(&blackTime, sizeof(blackTime));
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
	usleep(500000);
	game = game->pv;
	b = tb;
	w = tw;
}

void	Display::run()
{
	if (b)
	{
		game->board[9][9] = 'b';
		game->turn = 'w';
	}
	refresh();
	game->rankMoves();
	SDL_Thread *thread;
 	thread = SDL_CreateThread(Display::TimerThread, "time", (void*)this);
 	SDL_DetachThread(thread);
 	thread = SDL_CreateThread(Display::preComp, "pre-compute", (void*)this);
 	SDL_DetachThread(thread);
	while (1)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN
				&& event.key.keysym.sym == SDLK_ESCAPE))
			{	
				game->freeGames({0, 0}, false);
				exit (1);
			}
			if (!isAITurn() && takeInput)
				checkClick();
			checkHint();
			checkHist();
		}
		if (game->trueWon)
			takeInput = false;
		else
			takeInput = true;
		if (takeInput && isAITurn())
		{
			Game *nextGame = game->aiMove();
			game = nextGame;
			if (game->turn == 'w')
				bzero(&whiteTime, sizeof(whiteTime));
			else
				bzero(&blackTime, sizeof(blackTime));
			AIPlayed = true;
			guessMv.clear();
			game->pv->freeGames(game->lastMv, true);
cout << endl << endl;
		}
	}
}
