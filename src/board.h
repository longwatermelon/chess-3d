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

    void rotate(glm::vec3 rot);

private:
    glm::vec3 m_pos, m_rot;
    // YXZ
    Piece board[8][8][8];
    std::vector<Model> m_models;
};

