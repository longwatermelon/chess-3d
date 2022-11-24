#include "render.h"
#include <SDL2/SDL.h>

static glm::vec2 project(glm::vec3 p)
{
    return glm::vec2(
        ((p.x / p.z) + .5f) * SCRSIZE,
        ((p.y / p.z) + .5f) * SCRSIZE
    );
}

struct SCInfo
{
    float x, z, sx, sz;
};

static void scanline(int ty, int by, SCInfo &s1, SCInfo &s2, SDL_Color color, uint32_t *scr, float *zbuf)
{
    uint32_t hex = 0x00000000 | color.r << 16 | color.g << 8 | color.b;

    for (int y = ty; y < by; ++y)
    {
        float z = s1.z;
        float sz = (s2.z - s1.z) / (s2.x - s1.x);

        for (int x = std::min(s1.x, s2.x); x < std::max(s1.x, s2.x); ++x)
        {
            z += sz;

            if (x < 0)
            {
                z += sz * -x;
                x = -1;
                continue;
            }

            if (x >= SCRSIZE) break;

            int i = y * SCRSIZE + x;
            if (i >= 0 && i <= SCRSIZE * SCRSIZE)
            {
                if (z < zbuf[i])
                {
                    zbuf[i] = z;
                    scr[i] = hex;
                }
            }
        }

        s1.x += s1.sx;
        s2.x += s2.sx;

        s1.z += s1.sz;
        s2.z += s2.sz;
    }
}

void rend::triangle(Tri t, uint32_t *scr, float *zbuf)
{
    if (t.verts[0].z <= .5f || t.verts[1].z <= .5f || t.verts[2].z <= .5f)
        return;

    std::array<glm::vec3, 3> proj;
    for (int i = 0; i < 3; ++i)
    {
        glm::vec2 p = project(t.verts[i]);
        proj[i] = glm::vec3(p.x, p.y, t.verts[i].z);
    }

    if (proj[0].y > proj[1].y) std::swap(proj[0], proj[1]);
    if (proj[0].y > proj[2].y) std::swap(proj[0], proj[2]);
    if (proj[1].y > proj[2].y) std::swap(proj[1], proj[2]);

    auto gen_sc = [proj](int a, int b){
        return SCInfo{
            .x = roundf(proj[a].x), .z = proj[a].z,
            .sx = (proj[b].x - proj[a].x) / (proj[b].y - proj[a].y),
            .sz = (proj[b].z - proj[a].z) / (proj[b].y - proj[a].y)
        };
    };

    SCInfo s02 = gen_sc(0, 2),
           s01 = gen_sc(0, 1),
           s12 = gen_sc(1, 2);

    scanline(proj[0].y, proj[1].y, s02, s01, t.color, scr, zbuf);
    scanline(proj[1].y, proj[2].y, s02, s12, t.color, scr, zbuf);
}

