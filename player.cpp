#include "player.h"
#include "constants.h"
#include "grid.h"
#include "board.h"

Player::Player(int color) : color(color)
{

}

void Player::setBoard(Board *board)
{
    this->board = board;
    this->posMat = this->board->getPositionMatrix();
}


bool Player::isMoveValid(int piece, int r1, int c1, int r2, int c2)
{
    int self = piece;
    int op = ALT(piece);

    if (posMat->get(r2, c2) == self) return false;

    if (r1 == r2)
    {
        if (c1 < c2)
        {
            for (int i = c1+1; i < c2; i++)
                if (posMat->get(r1, i) == op) return false;
        }
        else
        {
            for (int i = c1-1; i > c2; i--)
                if (posMat->get(r1, i) == op) return false;
        }
    }
    else if (c1 == c2)
    {
        if (r1 < r2)
        {
            for (int i = r1+1; i < r2; i++)
                if (posMat->get(i, c1) == op) return false;
        }
        else
        {
            for (int i = r1-1; i > r2; i--)
                if (posMat->get(i, c1) == op) return false;
        }
    }
    else if (r1 > r2 && c1 < c2)
    {
        for (int i = r1-1, j = c1+1; i > r2 && j < c2; i--, j++)
            if (posMat->get(i, j) == op) return false;
    }
    else if (r1 < r2 && c1 > c2)
    {
        for (int i = r1+1, j = c1-1; i < r2 && j > c2; i++, j--)
            if (posMat->get(i, j) == op) return false;
    }
    else if (r1 > r2 && c1 > c2)
    {
        for (int i = r1-1, j = c1-1; i > r2 && j > c2; i--, j--)
            if (posMat->get(i, j) == op) return false;
    }
    else if (r1 < r2 && c1 < c2)
    {
        for (int i = r1+1, j = c1+1; i < r2 && j < c2; i++, j++)
            if (posMat->get(i, j) == op) return false;
    }
    return true;
}


int Player::getColor()
{
    return color;
}


