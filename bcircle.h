#ifndef BCIRCLE_H
#define BCIRCLE_H

#include "constants.h"
#include <QGraphicsEllipseItem>

class BCircle : public QGraphicsEllipseItem
{
private:
    int row, col;
public:
    BCircle(int row, int col, bool white);
    void setPos(int row, int col);
    int getRow();
    int getCol();
};

#endif // BCIRCLE_H
