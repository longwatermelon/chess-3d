#pragma once
#include "model.h"

enum class PieceType
{
    NONE = 0,
    PAWN = 1
};

enum class Color
{
    WHITE,
    BLACK
};

struct Piece
{
    PieceType type{ PieceType::NONE };
    Color color{ Color::WHITE };
};

class Board
{
public:
    Board(glm::vec3 pos, const std::string &res_prefix);
    ~Board();

    void render(uint32_t *scr, float *zbuf);
    glm::ivec3 raycast(int mx, int my);

    void rotate(glm::vec3 rot);

    void select(glm::ivec3 coord);
    std::vector<glm::ivec3> possible_moves(glm::ivec3 coord);

private:
    glm::vec3 m_pos, m_rot;
    // YXZ
    Piece m_board[8][8][8];
    std::vector<Model> m_models;

    glm::ivec3 m_selected{ -1 };
    std::vector<glm::ivec3> m_moves;
};

