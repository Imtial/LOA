#include "grid.h"
#include "constants.h"
#include "cell.h"

Grid::Grid(int size) : pos(size, VB(size, 0)), neg(size, VB(size, 0)) {}

void Grid::set(int r, int c, int val)
{
    int size = pos.size();
    if (r < size && c < size)
    {
        if (val > 0) pos[r][c] = 1, neg[r][c] = 0;
        else if (val < 0) pos[r][c] = 0, neg[r][c] = 1;
        else pos[r][c] = 0, neg[r][c] = 0;
    }
}

void Grid::set(Cell &cell, int val)
{
    set(cell.r, cell.c, val);
}

int Grid::get(int r, int c)
{
    if (pos[r][c]) return 1;
    if (neg[r][c]) return -1;
    return 0;
}

int Grid::get(Cell &cell)
{
    return get(cell.r, cell.c);
}

void Grid::printGrid()
{
    int size = pos.size();
    QString str;
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
            str += QString::number(get(r, c)) + " ";
        qDebug() << str;
        str.clear();
    }
}

int Grid::size()
{
    return pos.size();
}
