#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "gomoku.hpp"
# include "Game.hpp"
# include <stack>
# include <SDL2/SDL.h>

# define W 1600
# define H 1000

struct timeFrame{
    int milli;
    int sec;
    int min;
};

class Display {
private:
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
    Game game;
    chrono::steady_clock::time_point begin;
    chrono::steady_clock::time_point end;
    Display();

public:
    stack<Game> hist;
    stack<Game> forward;
    Display(Game g);
    ~Display();
    void refresh();
    void run();
    void outputMove();
    void printNumber(int num, int x, int y, bool b, float scale);
    void updateTime(char color);
    void    checkClick();
    void    checkHist();
    void    checkHint();
    void    checkAIMove();
    void    capturedPieces();
    bool    isAITurn();

    timeFrame whiteTime;
    timeFrame blackTime;
    timeFrame whiteTotalTime;
    timeFrame blackTotalTime;
};

#endif
