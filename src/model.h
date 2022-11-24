#pragma once
#include "render.h"
#include <string>
#include <vector>

class Model
{
public:
    Model(glm::vec3 pos, glm::vec3 rot, const std::string &src);
    ~Model();

    void render(uint32_t *scr, float *zbuf);

    void move(glm::vec3 dir);
    void rotate(glm::vec3 rot);

private:
    glm::vec3 m_pos{ 0.f }, m_rot{ 0.f };
    std::vector<Tri> m_tris;
};

