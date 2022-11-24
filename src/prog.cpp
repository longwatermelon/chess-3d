#include "prog.h"
#include "render.h"
#include "model.h"
#include "rotate.h"

Prog::Prog(SDL_Window *w, SDL_Renderer *r)
    : m_window(w), m_rend(r)
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

    Tri t{
        .verts = {
            glm::vec3(0.f, -.5f, 5.f),
            glm::vec3(-.5f, .5f, 5.f),
            glm::vec3(.5f, .5f, 5.f)
        },
        .color = { 255, 255, 255 },
        .norm = { 1.f, 0.f, 0.f }
    };

    Model m("res/cube.obj");

    while (m_running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                m_running = false;
                break;
            }
        }

        for (int i = 0; i < 3; ++i)
        {
            /* t.verts[i].pos = rotate::point(t.verts[i].pos, glm::vec3(0.01f, 0.00f, .00f), glm::vec3(0.f, 0.f, 5.f)); */
        }

        SDL_RenderClear(m_rend);
        reset_buffers();

        rend::triangle(t, m_scr, m_zbuf);

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

