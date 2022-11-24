#pragma once
#include "render.h"
#include <string>
#include <vector>

class Model
{
public:
    Model(const std::string &src);
    ~Model();

    void render(uint32_t *scr, float *zbuf);

private:
    std::vector<Tri> m_tris;
};

