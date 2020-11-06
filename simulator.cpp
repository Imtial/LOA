#include "simulator.h"
#include <QEventLoop>
#include <QTimer>
#include <boardstate.h>
#include <constants.h>
#include <grid.h>
#include <cell.h>

typedef std::vector<Cell> VC;

Simulator::Simulator(BoardState &state, int color) : posMat(state.getGrid()),
    moveMat(posMat.size(), VB(posMat.size(), 0)), color(color)
{
    copiedState = new BoardState(state);
}

Simulator::~Simulator()
{
    delete copiedState;
}

bool Simulator::isMoveValid(int piece, const Cell &c1, const Cell &c2)
{
    int self = piece;
    int op = ALT(piece);

    if (posMat.get(c2.r, c2.c) == self) return false;

    if (c1.r == c2.r)
    {
        if (c1.c < c2.c)
        {
            for (int i = c1.c+1; i < c2.c; i++)
                if (posMat.get(c1.r, i) == op) return false;
        }
        else
        {
            for (int i = c1.c-1; i > c2.c; i--)
                if (posMat.get(c1.r, i) == op) return false;
        }
    }
    else if (c1.c == c2.c)
    {
        if (c1.r < c2.r)
        {
            for (int i = c1.r+1; i < c2.r; i++)
                if (posMat.get(i, c1.c) == op) return false;
        }
        else
        {
            for (int i = c1.r-1; i > c2.r; i--)
                if (posMat.get(i, c1.c) == op) return false;
        }
    }
    else if (c1.r > c2.r && c1.c < c2.c)
    {
        for (int i = c1.r-1, j = c1.c+1; i > c2.r && j < c2.c; i--, j++)
            if (posMat.get(i, j) == op) return false;
    }
    else if (c1.r < c2.r && c1.c > c2.c)
    {
        for (int i = c1.r+1, j = c1.c-1; i < c2.r && j > c2.c; i++, j--)
            if (posMat.get(i, j) == op) return false;
    }
    else if (c1.r > c2.r && c1.c > c2.c)
    {
        for (int i = c1.r-1, j = c1.c-1; i > c2.r && j > c2.c; i--, j--)
            if (posMat.get(i, j) == op) return false;
    }
    else if (c1.r < c2.r && c1.c < c2.c)
    {
        for (int i = c1.r+1, j = c1.c+1; i < c2.r && j < c2.c; i++, j++)
            if (posMat.get(i, j) == op) return false;
    }
    return true;
}

void Simulator::generateMoveMat(int piece, int row, int col)
{
    int size = posMat.size();
    int rc = 0, cc = 0, pdc = 0, ndc = 0;
    int pcol, prow, ncol, nrow;

    if (col + row < size)
    {
        pcol = col + row;
        prow = 0;
    }
    else
    {
        pcol = size - 1;
        prow = row - (size - 1 - col);
    }

    if (col - row >= 0)
    {
        ncol = col - row;
        nrow = 0;
    }
    else
    {
        ncol = 0;
        nrow = row - col;
    }

    for (int i = 0; i < size; i++)
    {
        if (posMat.get(row, i)) rc++;
        if (posMat.get(i, col)) cc++;
        if (prow + i < size && pcol - i >= 0 && posMat.get(prow + i, pcol - i)) pdc++;
        if (nrow + i < size && ncol + i < size && posMat.get(nrow + i, ncol + i)) ndc++;
    }

    Cell curCell(row, col);

    if (col + rc < size && isMoveValid(piece, curCell, Cell(row, col+rc)))
        moveMat[row][col + rc] = true;

    if (col - rc >= 0 && isMoveValid(piece, curCell, Cell(row, col-rc)))
        moveMat[row][col - rc] = true;

    if (row + cc < size && isMoveValid(piece, curCell, Cell(row+cc, col)))
        moveMat[row + cc][col] = true;

    if (row - cc >= 0 && isMoveValid(piece, curCell, Cell(row-cc, col)))
        moveMat[row - cc][col] = true;

    if (row + pdc < size && col - pdc >= 0 && isMoveValid(piece, curCell, Cell(row+pdc, col-pdc)))
        moveMat[row + pdc][col - pdc] = true;

    if (row - pdc >= 0 && col + pdc < size && isMoveValid(piece, curCell, Cell(row-pdc, col+pdc)))
        moveMat[row - pdc][col + pdc] = true;

    if (row + ndc < size && col + ndc < size && isMoveValid(piece, curCell, Cell(row+ndc, col+ndc)))
        moveMat[row + ndc][col + ndc] = true;

    if (row - ndc >= 0 && col - ndc >= 0 && isMoveValid(piece, curCell, Cell(row-ndc, col-ndc)))
        moveMat[row - ndc][col - ndc] = true;
}

void Simulator::run()
{
    int size = posMat.size();
    VC pos;
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            if (posMat.get(r, c) == color) pos.push_back(Cell(r,c));
    Cell &selected = pos[rand() % pos.size()];
    emit squareSelected(selected.r, selected.c);


    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(1000);
    loop.exec();

    pos.clear();
    generateMoveMat(color, selected.r, selected.c);
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            if (moveMat[r][c]) pos.push_back(Cell(r,c));
    selected = pos[rand() % pos.size()];
    emit squareSelected(selected.r, selected.c);
}
