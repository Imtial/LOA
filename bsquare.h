#ifndef BSQUARE_H
#define BSQUARE_H

#include "constants.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#define DARKFILL    QColor(222, 170, 135)
#define LIGHTFILL   QColor(255, 221, 85)
#define HIGHLIGHT   QColor(72, 201, 176)

class Board;
class BCircle;

class BSquare : public QGraphicsRectItem
{
private:
    QBrush * brush;
    Board * board;
    BCircle * circle;
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
public:
    BSquare(int row, int col, Board * b);
    bool addCircle(BCircle * circle);
    void highlight();
    void clearHighlight();
    BCircle *getCircle();
};

#endif // BSQUARE_H
