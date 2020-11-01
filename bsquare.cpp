#include "bsquare.h"
#include "board.h"
#include <QGraphicsSceneMouseEvent>

BSquare::BSquare(int row, int col, Board * b)
    : QGraphicsRectItem((row+1)*DL, (col+1)*DL, DL, DL)
{
    setFlags(ItemIsSelectable);
    if ((row+col) % 2)
    {
        brush = new QBrush(DARKFILL);
    }
    else
    {
        brush = new QBrush(LIGHTFILL);
    }
    this->setBrush(*brush);
    this->board = b;
}

bool BSquare::addCircle(BCircle *circle)
{
//    if (this->circle != NULL && circle != NULL) return false;
    this->circle = circle;
    return true;
}

void BSquare::highlight()
{
    this->setBrush(QBrush(HIGHLIGHT));
}

void BSquare::clearHighlight()
{
    this->setBrush(*brush);
}

BCircle *BSquare::getCircle()
{
    return circle;
}

void BSquare::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
//        qDebug() << event->pos().x() << ", " << event->pos().y() << "\n";
        board->processClickEvent(event->pos().x(), event->pos().y());
    }
    else QGraphicsRectItem::mouseReleaseEvent(event);
}
