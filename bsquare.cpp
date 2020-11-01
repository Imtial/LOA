#include "bsquare.h"
#include "controller.h"
#include <QGraphicsSceneMouseEvent>

BSquare::BSquare(int row, int col, Controller * con)
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
    this->circle = NULL;
    this->setBrush(*brush);
    this->controller = con;
}

bool BSquare::addCircle(BCircle *circle)
{
//    if (this->circle != NULL && circle != NULL) return false;
    this->circle = circle;
    return true;
}

void BSquare::highlight(bool hl)
{
    if(hl) this->setBrush(QBrush(HIGHLIGHT));
    else this->setBrush(QBrush(SELECTION));
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
        controller->processClickEvent(event->pos().x(), event->pos().y());
    }
    else QGraphicsRectItem::mouseReleaseEvent(event);
}
