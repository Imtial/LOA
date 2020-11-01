#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player
{
public:
    Human(int color, Grid &posMat);
};

#endif // HUMAN_H
