#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "gomoku.hpp"
# include "Game.hpp"
# include <stack>
# include <SDL2/SDL.h>

# define W 1600
# define H 1000

struct timeFrame {
	int milli;
	int sec;
	int min;
};

class Display {
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Event event;
	SDL_Texture* background;
	SDL_Texture* white;
	SDL_Texture* black;
	SDL_Texture* left;
	SDL_Texture* right;
	SDL_Texture* num_b;
	SDL_Texture* num_w;
	SDL_Texture* blackWin;
	SDL_Texture* whiteWin;
	Game *game;

	bool takeInput = true;
	bool AIPlayed;
	Display();

public:
	stack<Game*> forward;
	Display(Game *g);
	~Display();
	void	printTime();
	void	refresh();
	void	run();
	void	outputMove();
	void	printNumber(int num, int x, int y, bool b, float scale);
	void	addTime(timeFrame *frame, float time);
	void	updateTime(char color, float time);
	void	checkClick();
	void	checkHist();
	void	checkHint();
	void	capturedPieces();
	bool	isAITurn();
	static int	TimerThread(void* param);
	static int	preComp(void* param);

	timeFrame whiteTime;
	timeFrame blackTime;
	timeFrame whiteTotalTime;
	timeFrame blackTotalTime;
};

#endif
