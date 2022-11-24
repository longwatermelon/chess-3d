#pragma once
#include <array>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#define SCRSIZE 600.f

struct Vertex
{
    glm::vec3 pos, norm;
};

struct Tri
{
    std::array<Vertex, 3> verts;
    SDL_Color color;
};

namespace rend
{
    void triangle(Tri t, uint32_t *scr, float *zbuf);
}

