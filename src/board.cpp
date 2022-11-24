#include "board.h"
#include "render.h"

Board::Board(glm::vec3 pos, const std::string &res_prefix)
    : m_pos(pos)
{
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            for (int z = 0; z < 8; ++z)
                board[y][x][z] = Piece::PAWN;

    /* for (int x = 0; x < 8; ++x) */
    /* { */
    /*     for (int z = 0; z < 8; ++z) */
    /*     { */
    /*         board[1][x][z] = Piece::PAWN; */
    /*     } */
    /* } */

    std::vector<std::string> pieces = {
        "pawn.obj"
    };

    for (auto &p : pieces)
        m_models.emplace_back(Model(res_prefix + p));
}

Board::~Board()
{
}

void Board::render(uint32_t *scr, float *zbuf)
{
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            for (int z = 0; z < 8; ++z)
            {
                if (board[y][x][z] != Piece::NONE)
                {
                    glm::vec3 pos = m_pos + glm::vec3(x - 3.5, y - 3.5, z - 3.5);
                    m_models[(int)board[y][x][z] - 1].render(pos, m_rot, m_pos, scr, zbuf);
                }
            }
        }
    }
}

void Board::rotate(glm::vec3 rot)
{
    m_rot += rot;
}

