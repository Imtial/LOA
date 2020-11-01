#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player
{
public:
    Human(int color);
    void move(int row, int col);
    QString getType();
};

#endif // HUMAN_H
