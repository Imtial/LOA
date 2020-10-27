#include "bcircle.h"

BCircle::BCircle(int row, int col, bool white)
    : QGraphicsEllipseItem(0, 0, DD, DD)
{
//    this->row = row, this->col = col;
    this->setPos(row, col);
    if (white) this->setBrush(Qt::white);
    else this->setBrush(Qt::black);
}

void BCircle::setPos(int row, int col)
{
    this->row = row; this->col = col;
//    qDebug() << this->x() << ", " << this->y();
    QGraphicsEllipseItem::setPos((col+1)*DL+OFFSET, (row+1)*DL+OFFSET);
//    qDebug() << this->x() << ", " << this->y();
}

int BCircle::getRow()
{
    return row;
}

int BCircle::getCol()
{
    return col;
}
