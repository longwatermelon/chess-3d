#include "prog.h"
#include "render.h"
#include "model.h"
#include "rotate.h"

Prog::Prog(SDL_Window *w, SDL_Renderer *r)
    : m_window(w), m_rend(r), m_board(glm::vec3(0.f, 0.f, 15.f), "res/")
{
    m_scrtex = SDL_CreateTexture(m_rend,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        SCRSIZE, SCRSIZE);
    reset_buffers();
}

Prog::~Prog()
{
    SDL_DestroyTexture(m_scrtex);
}

void Prog::mainloop()
{
    SDL_Event evt;

    while (m_running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (evt.button.button == SDL_BUTTON_LEFT)
                {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    glm::ivec3 res = m_board.raycast(mx, my);
                    m_board.select(res);
                }
                if (evt.button.button == SDL_BUTTON_RIGHT)
                {
                    m_rmb = true;
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (evt.button.button == SDL_BUTTON_RIGHT)
                {
                    m_rmb = false;
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    SDL_WarpMouseInWindow(m_window, SCRSIZE / 2, SCRSIZE / 2);
                }
                break;
            case SDL_MOUSEMOTION:
                if (m_rmb)
                    m_board.rotate(glm::vec3(-evt.motion.yrel / 100.f, -evt.motion.xrel / 100.f, 0.f));
                break;
            case SDL_MOUSEWHEEL:
                m_board.zoom(-evt.wheel.y / 5.f);
                break;
            }
        }

        SDL_RenderClear(m_rend);
        reset_buffers();

        m_board.render(m_scr, m_zbuf);

        SDL_UpdateTexture(m_scrtex, 0, m_scr, 600 * sizeof(uint32_t));
        SDL_RenderCopy(m_rend, m_scrtex, 0, 0);

        SDL_SetRenderDrawColor(m_rend, 255, 0, 0, 255);
        SDL_RenderPresent(m_rend);
    }
}

void Prog::reset_buffers()
{
    for (int i = 0; i < SCRSIZE * SCRSIZE; ++i)
    {
        m_scr[i] = 0x00000000;
        m_zbuf[i] = INFINITY;
    }
}

