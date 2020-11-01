#ifndef AI_H
#define AI_H

#include "player.h"
#include <QPair>
#include <vector>
#include <ctime>

typedef std::vector<int> VI;
typedef std::vector<std::vector<int>> VVI;
typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;
typedef QPair<int, int> PII;
typedef std::vector<PII> VPII;

class Board;

class AI : public Player
{
private:
public:
    AI(int color, Grid &posMat);
};

#endif // AI_H
