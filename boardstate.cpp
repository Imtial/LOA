#include "boardstate.h"
#include "constants.h"
#include "grid.h"
#include <iomanip>
#include <cmath>

#define DIAG    8

BoardState::BoardState(Grid &posMat)
    : wQuadMat(posMat.size()+1, VI(posMat.size() + 1)),
    bQuadMat(posMat.size()+1, VI(posMat.size() + 1)), wRaw(), bRaw(),
    wCom(), bCom(), wCount(0), bCount(0), wE(0), bE(0), wPieces(), bPieces(),
    wAvgDist(0), bAvgDist(0), wCen(0), bCen(0), wEdge(0), bEdge(0)
{
    this->posMat = &posMat;

    // Quad calculation
    int n = wQuadMat.size();

    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            int s = quadType(r, c, W);
            wQuadMat[r][c] = s;
            if (s == 1) wE += 1;
            if (s == 3) wE -= 1;
            if (s == DIAG) wE -= 2;

            s = quadType(r, c, B);
            bQuadMat[r][c] = s;
            if (s == 1) bE += 1;
            if (s == 3) bE -= 1;
            if (s == DIAG) bE -= 2;
        }
    }

    wE /= 4;
    bE /= 4;

    // Center-Of-Mass calculation
    initCOM();

    // Average Distance Calculation
    recalculateDistMetrics();
}

int BoardState::quadType(int r, int c, int color)
{
    int last = wQuadMat.size() - 1;
    int count = 0;
    if (r == last)
    {
        if (c == 0 || c == last) return 0;
        else
        {
            if (posMat->get(r-1, c) == color) count++;
            if (posMat->get(r-1, c-1) == color) count++;
            return count;
        }
    }
    else if (c == last)
    {
        if (r == 0) return 0;
        else
        {
            if (posMat->get(r, c-1) == color) count++;
            if (posMat->get(r-1, c-1) == color) count++;
            return count;
        }
    }
    else if (r == 0)
    {
        if (c == 0) return 0;
        else
        {
            if (posMat->get(r, c) == color) count++;
            if (posMat->get(r, c-1) == color) count++;
            return count;
        }
    }
    else if (c == 0)
    {
        if (posMat->get(r, c) == color) count++;
        if (posMat->get(r-1, c) == color) count++;
        return count;
    }
    else
    {
        if (posMat->get(r, c) == color) count++;
        if (posMat->get(r, c-1) == color) count++;
        if (posMat->get(r-1, c) == color) count++;
        if (posMat->get(r-1, c-1) == color) count++;

        if (count == 2)
        {
            if ((posMat->get(r, c) == color && posMat->get(r-1, c-1) == color)
                    || (posMat->get(r, c-1) == color && posMat->get(r-1, c) == color))
                return DIAG;
        }
        return count;
    }
}


float BoardState::updateQuadScore(int r, int c, int color)
{
    float e = 0;
    for (int i = r; i <= r+1; i++)
        for (int j = c; j <= c+1; j++)
        {
            int s = quadType(i, j, color);

            if (color == W) wQuadMat[i][j] = s;
            else if (color == B) bQuadMat[i][j] = s;

            if (s == 1) e += 1;
            else if (s == 3) e -= 1;
            else if (s == DIAG) e -= 2;
        }
    return e/4;
}

void BoardState::updateCOM(Cell &init, int initCol, Cell &target, int tarCol)
{
    if (initCol == W)
    {
        wRaw += target - init;
        if (tarCol == B) bRaw -= target;
    }
    else if (initCol == B)
    {
        bRaw += target - init;
        if (tarCol == W) wRaw -= target;
    }
    wCom = wRaw / wCount;
    bCom = bRaw / bCount;
}

void BoardState::recalculateDistMetrics()
{
    wAvgDist = 0, bAvgDist = 0;
    wCen = 0, bCen = 0;
    wEdge = 0, bEdge = 0;
    for (Cell &cell : wPieces)
    {
        int dist = getCellDist(cell, wCom);
        if (dist < 2) wCen++;
        else if (isEdge(cell, 1)) wEdge++;
        wAvgDist += dist;
    }
    for (Cell &cell : bPieces)
    {
        int dist = getCellDist(cell, bCom);
        if (dist < 2) bCen++;
        else if (isEdge(cell, 1)) bEdge++;
        bAvgDist += dist;
    }
    wAvgDist = float(wAvgDist) / wCount + 0.5;
    bAvgDist = float(bAvgDist) / bCount + 0.5;
}

int BoardState::getCenScoreChange(Cell &init, int initCol, Cell &target, int tarCol)
{
    if (initCol == W)
    {
        int dPrev = getCellDist(init, wCom);
        int dNext = getCellDist(target, wCom);

        if (dNext < 3 && dPrev > 2) wCen++;
        else if (dNext > 2 && dPrev < 3)
        {
            wCen--;
            if (isEdge(target, 1)) wEdge++;
        }
        if (tarCol == B)
        {
            bCen--;
        }
    }
    else if (initCol == B)
    {
        int dPrev = getCellDist(init, bCom);
        int dNext = getCellDist(target, bCom);

        if (dNext < 3 && dPrev > 2) bCen++;
        else if (dNext > 2 && dPrev < 3)
        {
            bCen--;
            if (isEdge(target, 1)) bEdge++;
        }
        if (tarCol == B)
        {
            wCen--;
        }
    }
}

int BoardState::getCellDist(Cell &c1, Cell &c2)
{
    int rDiff = abs(c1.r - c2.r);
    int cDiff = abs(c1.c - c2.c);
    if (rDiff > cDiff) return rDiff;
    return cDiff;
}

bool BoardState::isEdge(Cell &cell, int d)
{
    int n = posMat->size();

    return cell.r < d || cell.r >= n-d || cell.c < d || cell.c >= n-d;
}

float BoardState::cellScore(int r, int c, int color)
{
    VVI &quadMat = (color == W) ? wQuadMat : bQuadMat;

    float e = 0;
    for (int i = r; i <= r+1; i++)
        for (int j = c; j <= c+1; j++)
        {
            int s = quadMat[i][j];
            if (s == 1) e += 1;
            else if (s == 3) e -= 1;
            else if (s == DIAG) e -= 2;
        }
    return e/4;
}

void BoardState::update(Cell &init, int initCol, Cell &target, int tarCol)
{
    // update euler value and Quad Matrix
    float *posE = NULL, *negE = NULL;
    bool captured = false;
    if (initCol == W)
    {
        posE = &wE;
        if (tarCol == B)
        {
            negE = &bE;
            captured = true;
            bPieces.remove(target);
            bCount--;
        }
    }
    else if (initCol == B)
    {
        posE = &bE;
        if (tarCol == W)
        {
            negE = &wE;
            captured = true;
            wPieces.remove(target);
            wCount--;
        }
    }
    if (posE == NULL) return;

    posMat->set(init, initCol);
    posMat->set(target, tarCol);
    float prevPosE, newPosE, capE;

    prevPosE = cellScore(init.r, init.c, initCol);
    newPosE = cellScore(target.r, target.c, initCol);
    capE = cellScore(target.r, target.c, -initCol);

    posMat->set(init, 0);
    posMat->set(target, initCol);

    prevPosE -= updateQuadScore(init.r, init.c, initCol);
    newPosE -= updateQuadScore(target.r, target.c, initCol);
    capE -= updateQuadScore(target.r, target.c, -initCol);

    if (captured) *negE -= capE;
    *posE -= (prevPosE + newPosE);

    posMat->set(init, 0);
    posMat->set(target, initCol);

    // update COM
    updateCOM(init, initCol, target, tarCol);
}

bool BoardState::terminal(int color, int sr, int sc)
{
    int total = (color == W) ? wCount : bCount;
    Grid copyMat = *posMat;
    int size = copyMat.size();
    LC cells;

    cells.push_back(Cell(sr, sc));

    while (!cells.empty())
    {
        Cell cell = cells.back();
        cells.pop_back();

//        qDebug() << cell.r << ", " << cell.c;

        if (cell.r+1 < size && cell.c+1 < size && copyMat.get(cell.r+1, cell.c+1) == color)
            cells.push_back(Cell(cell.r+1, cell.c+1));

        if (cell.r+1 < size && cell.c-1 >= 0 && copyMat.get(cell.r+1, cell.c-1) == color)
            cells.push_back(Cell(cell.r+1, cell.c-1));

        if (cell.r-1 >= 0 && cell.c+1 < size && copyMat.get(cell.r-1, cell.c+1) == color)
            cells.push_back(Cell(cell.r-1, cell.c+1));

        if (cell.r-1 >= 0 && cell.c-1 >= 0 && copyMat.get(cell.r-1, cell.c-1) == color)
            cells.push_back(Cell(cell.r-1, cell.c-1));

        if (cell.c+1 < size && copyMat.get(cell.r, cell.c+1) == color)
            cells.push_back(Cell(cell.r, cell.c+1));

        if (cell.c-1 >= 0 && copyMat.get(cell.r, cell.c-1) == color)
            cells.push_back(Cell(cell.r, cell.c-1));

        if (cell.r+1 < size && copyMat.get(cell.r+1, cell.c) == color)
            cells.push_back(Cell(cell.r+1, cell.c));

        if (cell.r-1 >= 0 && copyMat.get(cell.r-1, cell.c) == color)
            cells.push_back(Cell(cell.r-1, cell.c));

        copyMat.set(cell, 0);
        total--;
    }
    if (total > 0) return false;
    return true;
}

float BoardState::euler(int color)
{
    if (color == W) return wE;
    return bE;
}

BoardState::BoardState(const BoardState &boardState)
{
    posMat = new Grid(boardState.posMat->size());
    *posMat = *(boardState.posMat);
    wQuadMat = boardState.wQuadMat;
    bQuadMat = boardState.bQuadMat;
    wRaw = boardState.wRaw;
    bRaw = boardState.bRaw;
    wCom = boardState.wCom;
    bCom = boardState.bCom;
    wCount = boardState.wCount;
    bCount = boardState.bCount;
    wE = boardState.wE;
    bE = boardState.bE;
    wPieces = boardState.wPieces;
    bPieces = boardState.bPieces;
    wAvgDist = boardState.wAvgDist;
    bAvgDist = boardState.bAvgDist;
    wCen = boardState.wCen;
    bCen = boardState.bCen;
    wEdge = boardState.wEdge;
    bEdge = boardState.bEdge;
}

BoardState& BoardState::operator=(const BoardState &boardState)
{
    posMat = new Grid(boardState.posMat->size());
    *posMat = *(boardState.posMat);
    wQuadMat = boardState.wQuadMat;
    bQuadMat = boardState.bQuadMat;
    wRaw = boardState.wRaw;
    bRaw = boardState.bRaw;
    wCom = boardState.wCom;
    bCom = boardState.bCom;
    wCount = boardState.wCount;
    bCount = boardState.bCount;
    wE = boardState.wE;
    bE = boardState.bE;
    wPieces = boardState.wPieces;
    bPieces = boardState.bPieces;
    wAvgDist = boardState.wAvgDist;
    bAvgDist = boardState.bAvgDist;
    wCen = boardState.wCen;
    bCen = boardState.bCen;
    wEdge = boardState.wEdge;
    bEdge = boardState.bEdge;
    return *this;
}

void BoardState::initCOM()
{
    int n = posMat->size();
    wRaw.r = 0, wRaw.c = 0;
    bRaw.r = 0, bRaw.c = 0;
    if (!wPieces.empty()) wPieces.clear();
    if (!bPieces.empty()) bPieces.clear();
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            if (posMat->get(r, c) == W)
            {
                wCount++;
                wRaw.r += r;
                wRaw.c += c;
                wPieces.push_back(Cell(r, c));
            }
            else if (posMat->get(r, c) == B)
            {
                bCount++;
                bRaw.r += r;
                bRaw.c += c;
                bPieces.push_back(Cell(r, c));
            }
        }
    }
//    qDebug() << "W-COM=(" << wCom.r << ", " << wCom.c << ")";
//    qDebug() << "B-COM=(" << bCom.r << ", " << bCom.c << ")";
    wCom = wRaw / wCount;
    bCom = bRaw / bCount;
//    qDebug() << "W-COM=(" << wCom.r << ", " << wCom.c << ")";
//    qDebug() << "B-COM=(" << bCom.r << ", " << bCom.c << ")";
}

void BoardState::print()
{
    int size = wQuadMat.size();
    QString head;
    for (int i = 0; i < size; i++)
    {
        head += " ";
    }
    qDebug() << head << "W-Q" << head << "||" << head << "B-Q" << head;
    for (int i = 0; i < size; i++)
    {
        QString row;
        for (int j = 0; j < size; j++)
        {
            if(wQuadMat[i][j] == DIAG) row += " D ";
            else row += " " + QString::number(wQuadMat[i][j]) + " ";
        }
        row += "  ||  ";
        for (int j = 0; j < size; j++)
        {
            if(bQuadMat[i][j] == DIAG) row += "D ";
            else row += " " + QString::number(bQuadMat[i][j]) + " ";
        }
        qDebug() << row;
    }
    qDebug() << "WHITE: COM=(" << wCom.r << ", " << wCom.c << "), E=" << wE << ", avg=" << wAvgDist << ", #=" << wCount;
    qDebug() << "BLACK: COM=(" << bCom.r << ", " << bCom.c << "), E=" << bE << ", avg=" << bAvgDist << ", #=" << bCount;
}

Grid &BoardState::getGrid()
{
    return *posMat;
}

BoardState::~BoardState()
{
    delete posMat;
}
