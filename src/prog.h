#pragma once
#include <SDL2/SDL.h>

class Prog
{
public:
    Prog(SDL_Window *w, SDL_Renderer *r);
    ~Prog();

    void mainloop();

private:
    bool m_running{ true };
    SDL_Window *m_window{ nullptr };
    SDL_Renderer *m_rend{ nullptr };
};

