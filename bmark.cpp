#include "bmark.h"

BMark::BMark(bool row, bool first, int i, int size)
    : QGraphicsSimpleTextItem()
{
    int x = 0, y = 0;
    if (row == true)
    {
        QString label('A'+i);
        this->setText(label);
        y = (i+1) * DL + DL/2;
        if (first) x = 3 * DL/4;
        else x = (size+1) * DL + DL/5;
    }
    else
    {
        QString label('1'+i);
        this->setText(label);
        x = (i+1) * DL + DL/2;
        if (first) y = 3 * DL/4;
        else y = (size+1) * DL + DL/5;
    }
    this->setX(x);
    this->setY(y);
}
