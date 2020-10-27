#ifndef BMARK_H
#define BMARK_H

#include "constants.h"
#include <QGraphicsSimpleTextItem>

class BMark : public QGraphicsSimpleTextItem
{
public:
    BMark(bool rowOrCol, bool first, int i, int size = SIZE);
};

#endif // BMARK_H
