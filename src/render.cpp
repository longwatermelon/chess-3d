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
    glm::vec3 norm, snorm;
};

static void scanline(int ty, int by, SCInfo &s1, SCInfo &s2, SDL_Color color, uint32_t *scr, float *zbuf)
{
    uint32_t hex = 0x00000000 | color.r << 16 | color.g << 8 | color.b;

    for (int y = ty; y < by; ++y)
    {
        float z = s1.z;
        float sz = (s2.z - s1.z) / (s2.x - s1.x);

        glm::vec3 norm = s1.norm;
        glm::vec3 snorm = (s2.norm - s1.norm) / (s2.x - s1.x);

        for (int x = std::min(s1.x, s2.x); x < std::max(s1.x, s2.x); ++x)
        {
            z += sz;
            norm += snorm;

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
                    scr[i] = 0x00000000 | (int)(norm.x * 255.f) << 16 | (int)(norm.y * 255.f) << 8 | (int)(norm.z * 255.f);
                }
            }
        }

        s1.x += s1.sx;
        s2.x += s2.sx;

        s1.z += s1.sz;
        s2.z += s2.sz;

        s1.norm += s1.snorm;
        s2.norm += s2.snorm;
    }
}

void rend::triangle(Tri t, uint32_t *scr, float *zbuf)
{
    if (t.verts[0].pos.z <= .5f || t.verts[1].pos.z <= .5f || t.verts[2].pos.z <= .5f)
        return;

    std::array<Vertex, 3> proj;
    for (int i = 0; i < 3; ++i)
    {
        glm::vec2 p = project(t.verts[i].pos);
        proj[i] = Vertex{
            .pos = glm::vec3(p.x, p.y, t.verts[i].pos.z),
            .norm = t.verts[i].norm
        };
    }

    if (proj[0].pos.y > proj[1].pos.y) std::swap(proj[0], proj[1]);
    if (proj[0].pos.y > proj[2].pos.y) std::swap(proj[0], proj[2]);
    if (proj[1].pos.y > proj[2].pos.y) std::swap(proj[1], proj[2]);

    auto gen_sc = [proj](int a, int b){
        float dy = proj[b].pos.y - proj[a].pos.y;
        return SCInfo{
            .x = roundf(proj[a].pos.x), .z = proj[a].pos.z,
            .sx = (proj[b].pos.x - proj[a].pos.x) / dy,
            .sz = (proj[b].pos.z - proj[a].pos.z) / dy,
            .norm = proj[a].norm,
            .snorm = (proj[b].norm - proj[a].norm) / dy
        };
    };

    SCInfo s02 = gen_sc(0, 2),
           s01 = gen_sc(0, 1),
           s12 = gen_sc(1, 2);

    scanline(proj[0].pos.y, proj[1].pos.y, s02, s01, t.color, scr, zbuf);
    scanline(proj[1].pos.y, proj[2].pos.y, s02, s12, t.color, scr, zbuf);
}

