#ifndef AI_H
#define AI_H

#include "player.h"

class AI : public Player
{
private:
public:
    AI(int color);
    void move(int row, int col);
    QString getType();
};

#endif // AI_H
