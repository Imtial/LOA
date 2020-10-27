#ifndef AI_H
#define AI_H

#include <QPair>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<std::vector<int>> VVI;
typedef QPair<int, int> PII;

class AI
{
    VVI &posMat;
public:
    AI(VVI &posMat);
    void move();
};

#endif // AI_H
