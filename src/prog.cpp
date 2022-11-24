#include "prog.h"
#include "render.h"

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
            Vertex{ .pos = glm::vec3(0.f, 0.f, 5.f), .norm = glm::vec3(1.f, 0.f, 0.f) },
            Vertex{ .pos = glm::vec3(1.f, 0.f, 5.f), .norm = glm::vec3(0.f, 1.f, 0.f) },
            Vertex{ .pos = glm::vec3(.5f, 1.f, 5.f), .norm = glm::vec3(0.f, 0.f, 1.f) }
        },
        .color = { 255, 255, 255 }
    };

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

