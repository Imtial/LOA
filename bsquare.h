#ifndef BSQUARE_H
#define BSQUARE_H

#include "constants.h"
#include <QGraphicsRectItem>

#define DARKFILL    QColor(222, 170, 135)
#define LIGHTFILL   QColor(255, 221, 85)
#define HIGHLIGHT   QColor(72, 201, 176)
#define SELECTION   QColor(156, 39, 176)

//class Board;
class Controller;
class QGraphicsSceneMouseEvent;
class BCircle;

class BSquare : public QGraphicsRectItem
{
private:
    QBrush * brush;
//    Board * board;
    Controller * controller;
    BCircle * circle;
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
public:
    BSquare(int row, int col, Controller * con);
    bool addCircle(BCircle * circle);
    void highlight(bool hl = true);
    void clearHighlight();
    BCircle *getCircle();
};

#endif // BSQUARE_H
