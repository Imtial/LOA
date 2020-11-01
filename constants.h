#ifndef BOARDBLOCK_H
#define BOARDBLOCK_H

#include <QColor>
#include <QBrush>
#include <QDebug>


#define W   1
#define B   -1
#define ALT(p) -p

// Number of Rows & Columns
#define SIZE 6
// width & height of each square block
#define DL  80
// offset of the topLeftX & topLeftY of circle from square block
#define OFFSET  10
// diameter of the circle = DL - 2 * OFFSET
#define DD  60

#endif // BOARDBLOCK_H
