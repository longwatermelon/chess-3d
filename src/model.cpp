#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>

Model::Model(const std::string &src)
{
    std::ifstream ifs(src);
    std::string buf;
    // Skip to first line of data
    for (int i = 0; i < 5; ++i) std::getline(ifs, buf);

    std::vector<glm::vec3> verts, norms;

    while (buf.substr(0, 2) == "v ")
    {
        glm::vec3 v;
        std::stringstream ss(buf);
        char c;
        ss >> c >> v.x >> v.y >> v.z;
        verts.emplace_back(v);
        std::getline(ifs, buf);
    }

    while (buf.substr(0, 2) == "vn")
    {
        glm::vec3 n;
        std::string s;
        std::stringstream ss(buf);
        ss >> s >> n.x >> n.y >> n.z;
        std::getline(ifs, buf);
    }

    while (buf[0] != 'f') std::getline(ifs, buf);

    do
    {
        std::stringstream ss(buf);
        char c;
    } while (std::getline(ifs, buf));
}

Model::~Model()
{
}

void Model::render(uint32_t *scr, float *zbuf)
{
}

