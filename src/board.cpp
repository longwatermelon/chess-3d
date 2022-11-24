#include "board.h"
#include "rotate.h"
#include "render.h"
#include <algorithm>

Board::Board(glm::vec3 pos, const std::string &res_prefix)
    : m_pos(pos)
{
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            for (int z = 0; z < 8; ++z)
            {
                m_board[y][x][z] = Piece{ .type = PieceType::NONE };

                if (y == 6) m_board[y][x][z] = Piece{ .type = PieceType::PAWN, .color = Color::WHITE };
                if (y == 1) m_board[y][x][z] = Piece{ .type = PieceType::PAWN, .color = Color::BLACK };
            }

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
                bool in_moves = std::find(m_moves.begin(), m_moves.end(), glm::ivec3(x, y, z)) != m_moves.end();
                PieceType type = in_moves ? m_board[m_selected.y][m_selected.x][m_selected.z].type : m_board[y][x][z].type;
                if (m_board[y][x][z].type != PieceType::NONE || in_moves)
                {
                    glm::vec3 pos = m_pos + glm::vec3(x - 3.5, y - 3.5, z - 3.5);
                    SDL_Color color = m_selected == glm::ivec3(x, y, z) ? SDL_Color{ 255, 0, 0 } :
                        (m_board[y][x][z].color == Color::WHITE ? SDL_Color{ 255, 255, 255 } : SDL_Color{ 50, 50, 50 });
                    if (in_moves) color = { 0, 255, 0 };
                    m_models[(int)type - 1].render(pos, m_rot, m_pos, color, scr, zbuf);
                }
            }
        }
    }
}

glm::ivec3 Board::raycast(int mx, int my)
{
    float ha = ((float)mx / SCRSIZE) - .5f;
    float va = ((float)my / SCRSIZE) - .5f;
    float px = std::sin(ha);
    float py = std::sin(va);
    glm::vec3 dir = glm::normalize(glm::vec3(px, py, 1.f));

    float nearest = INFINITY;
    glm::ivec3 res(-1.f);
    bool selected_possible_move = false;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            for (int z = 0; z < 8; ++z)
            {
                bool in_moves = std::find(m_moves.begin(), m_moves.end(), glm::ivec3(x, y, z)) != m_moves.end();
                PieceType type = in_moves ? m_board[m_selected.y][m_selected.x][m_selected.z].type : m_board[y][x][z].type;
                if (m_board[y][x][z].type != PieceType::NONE || in_moves)
                {
                    glm::vec3 pos = m_pos + glm::vec3(x - 3.5, y - 3.5, z - 3.5);

                    float t;
                    if (m_models[(int)type - 1].ray_intersect(dir, pos, m_rot, m_pos, &t) && t < nearest)
                    {
                        nearest = t;
                        res = glm::ivec3(x, y, z);
                        selected_possible_move = in_moves;
                    }
                }
            }
        }
    }

    if (selected_possible_move)
    {
        m_board[res.y][res.x][res.z] = m_board[m_selected.y][m_selected.x][m_selected.z];
        m_board[m_selected.y][m_selected.x][m_selected.z].type = PieceType::NONE;
        m_selected = glm::ivec3(-1.f);
        return glm::ivec3(-1.f);
    }

    return res;
}

void Board::rotate(glm::vec3 rot)
{
    m_rot += rot;
}

void Board::select(glm::ivec3 coord)
{
    m_selected = coord;
    m_moves = possible_moves(m_selected);
}

std::vector<glm::ivec3> Board::possible_moves(glm::ivec3 coord)
{
    Piece p = m_board[coord.y][coord.x][coord.z];
    std::vector<glm::ivec3> moves;

    switch (p.type)
    {
    case PieceType::PAWN:
        moves.emplace_back(glm::vec3(coord.x, coord.y + (p.color == Color::WHITE ? -1 : 1), coord.z));
        if ((coord.y == 1 && p.color == Color::BLACK) || (coord.y == 6 && p.color == Color::WHITE))
            moves.emplace_back(glm::vec3(coord.x, coord.y + (p.color == Color::WHITE ? -2 : 2), coord.z));
        break;
    default: break;
    }

    return moves;
}

