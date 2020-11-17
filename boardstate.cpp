#include "boardstate.h"
#include "constants.h"
#include "grid.h"
#include <iomanip>
#include <cmath>

#define DIAG    8

int pieceSquareTable[8][8] =
{
{-80, -25, -20, -20, -20, -20, -25, -80},
{-25,  10,  10,  10,  10,  10,  10,  -25},
{-20,  10,  25,  25,  25,  25,  10,  -20},
{-20,  10,  25,  50,  50,  25,  10,  -20},
{-20,  10,  25,  50,  50,  25,  10,  -20},
{-20,  10,  25,  25,  25,  25,  10,  -20},
{-25,  10,  10,  10,  10,  10,  10,  -25},
{-80, -25, -20, -20, -20, -20, -25, -80}
};

BoardState::BoardState(Grid &posMat)
    : wQuadMat(posMat.size()+1, VI(posMat.size() + 1)),
    bQuadMat(posMat.size()+1, VI(posMat.size() + 1)), wRaw(), bRaw(),
    wCom(), bCom(), wCount(0), bCount(0), wE(0), bE(0), wPieces(), bPieces(),
    wAvgDist(0), bAvgDist(0), wCen(0), bCen(0), wEdge(0), bEdge(0), wq3q4(0), bq3q4(0)
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
            else if (s == 3) wE -= 1, wq3q4++;
            else if (s == 4) wq3q4++;
            else if (s == DIAG) wE -= 2;

            s = quadType(r, c, B);
            bQuadMat[r][c] = s;
            if (s == 1) bE += 1;
            else if (s == 3) bE -= 1, bq3q4++;
            else if (s == 4) bq3q4++;
            else if (s == DIAG) bE -= 2;
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


float BoardState::updateQuads(int r, int c, int color)
{
    float e = 0;
    for (int i = r; i <= r+1; i++)
        for (int j = c; j <= c+1; j++)
        {
            int s = quadType(i, j, color);

            if (color == W)
            {
                if ((s == 4 || s == 3) && (wQuadMat[i][j] < 3 || wQuadMat[i][j] > 4)) wq3q4++;
                if ((wQuadMat[i][j] == 4 || wQuadMat[i][j] == 3) && (s < 3 || s > 4)) wq3q4--;
                wQuadMat[i][j] = s;
            }
            else if (color == B)
            {
                if ((s == 4 || s == 3) && (bQuadMat[i][j] < 3 || bQuadMat[i][j] > 4)) bq3q4++;
                if ((bQuadMat[i][j] == 4 || bQuadMat[i][j] == 3) && (s < 3 || s > 4)) bq3q4--;
                bQuadMat[i][j] = s;
            }

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
    recalculateDistMetrics();
}

void BoardState::recalculateDistMetrics()
{
    wAvgDist = 0, bAvgDist = 0;
    wCen = 0, bCen = 0;
    wEdge = 0, bEdge = 0;
    for (Cell &cell : wPieces)
    {
        float dist = getCellDist(cell, wCom);
        if (dist < 2) wCen++;
        else if (isEdge(cell, 1)) wEdge++;
        wAvgDist += dist;
    }
    for (Cell &cell : bPieces)
    {
        float dist = getCellDist(cell, bCom);
        if (dist < 2) bCen++;
        else if (isEdge(cell, 1)) bEdge++;
        bAvgDist += dist;
    }
    wAvgDist /= wCount;
    bAvgDist /= bCount;
}

void BoardState::getCenScoreChange(Cell &init, int initCol, Cell &target, int tarCol)
{
//    bool initEdge = isEdge(init, 1);
//    bool targetEdge = isEdge(target, 1);
    if (initCol == W)
    {
        int dPrev = getCellDist(init, wCom);
        int dNext = getCellDist(target, wCom);

//        if (initEdge) wEdge--;
//        if (targetEdge) wEdge++;
        if (dNext < 2 && dPrev > 1) wCen++;
        else if (dNext > 1 && dPrev < 2)
        {
            wCen--;
        }
        if (tarCol == B)
        {
            if (getCellDist(target, bCom) < 2) bCen--;
//            if (targetEdge) bEdge--;
        }
    }
    else if (initCol == B)
    {
        int dPrev = getCellDist(init, bCom);
        int dNext = getCellDist(target, bCom);

//        if (initEdge) bEdge--;
//        if (targetEdge) bEdge++;
        if (dNext < 2 && dPrev > 1) bCen++;
        else if (dNext > 1 && dPrev < 2)
        {
            bCen--;
        }
        if (tarCol == W)
        {
            if (getCellDist(target, wCom) < 2) wCen--;
//            if (targetEdge) wEdge--;
        }
    }
}

float BoardState::getCellDist(Cell &c1, Cell &c2)
{
    int rDiff = abs(c1.r - c2.r);
    int cDiff = abs(c1.c - c2.c);
    return sqrt(rDiff*rDiff + cDiff*cDiff);
}

bool BoardState::isEdge(Cell &cell, int d)
{
    int n = posMat->size();

    return cell.r < d || cell.r >= n-d || cell.c < d || cell.c >= n-d;
}

int BoardState::getPieceSquareValue(int color)
{
    int score = 0, x = 0;
    if (posMat->size() == 6) x = 1;
    if (color == W)
    {
        for (Cell& cell : wPieces)
        {
            score += pieceSquareTable[cell.r+x][cell.c+x];
        }
    }
    else if (color == B)
    {
        for (Cell& cell : bPieces)
        {
            score += pieceSquareTable[cell.r+x][cell.c+x];
        }
    }
    return score;
}

int BoardState::getMinArea(int color)
{

    int wMinR = 16, wMaxR = 0, wMinC = 16, wMaxC = 0;
    int bMinR = 16, bMaxR = 0, bMinC = 16, bMaxC = 0;

    if (color == W)
    {
        for (Cell &cell : wPieces)
        {
            if (cell.r > wMaxR) wMaxR = cell.r;
            if (cell.r < wMinR) wMinR = cell.r;
            if (cell.c > wMaxC) wMaxC = cell.c;
            if (cell.c < wMinC) wMinC = cell.c;
        }
        return (wMaxR - wMinR + 1) * (wMaxC - wMinC + 1);
    }
    if (color == B)
    {
        for (Cell &cell : bPieces)
        {
            if (cell.r > bMaxR) bMaxR = cell.r;
            if (cell.r < bMinR) bMinR = cell.r;
            if (cell.c > bMaxC) bMaxC = cell.c;
            if (cell.c < bMinC) bMinC = cell.c;
        }
        return (bMaxR - bMinR + 1) * (bMaxC - bMinC + 1);
    }
    return 0;
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
//        wPieces.remove(init);
        wPieces.erase(std::remove(wPieces.begin(), wPieces.end(), init), wPieces.end());
        wPieces.push_back(target);
        if (tarCol == B)
        {
            negE = &bE;
            captured = true;
//            bPieces.remove(target);
            bPieces.erase(std::remove(bPieces.begin(), bPieces.end(), target), bPieces.end());
            bCount--;
        }
    }
    else if (initCol == B)
    {
        posE = &bE;
//        bPieces.remove(init);
        bPieces.erase(std::remove(bPieces.begin(), bPieces.end(), init), bPieces.end());
        bPieces.push_back(target);
        if (tarCol == W)
        {
            negE = &wE;
            captured = true;
//            wPieces.remove(target);
            wPieces.erase(std::remove(wPieces.begin(), wPieces.end(), target), wPieces.end());
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

    prevPosE -= updateQuads(init.r, init.c, initCol);
    newPosE -= updateQuads(target.r, target.c, initCol);
    capE -= updateQuads(target.r, target.c, -initCol);

    if (captured) *negE -= capE;
    *posE -= (prevPosE + newPosE);

//    posMat->set(init, 0);
//    posMat->set(target, initCol);

    // update COM
    updateCOM(init, initCol, target, tarCol);
    getCenScoreChange(init, initCol, target, tarCol);
}

bool BoardState::terminal(int color, int sr, int sc)
{
    int total = (color == W) ? wCount : bCount;
    Grid copyMat = *posMat;
    int size = copyMat.size();
    VC cells;

    cells.push_back(Cell(sr, sc));

    while (!cells.empty())
    {
        Cell cell = cells.back();
        for (Cell &_c : cells) copyMat.set(_c, 0);
        cells.pop_back();
        total--;

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
    *this = boardState;
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
    wq3q4 = boardState.wq3q4;
    bq3q4 = boardState.bq3q4;
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
//    int size = wQuadMat.size();
//    QString head;
//    for (int i = 0; i < size; i++)
//    {
//        head += " ";
//    }
//    qDebug() << head << "W-Q" << head << "||" << head << "B-Q" << head;
//    for (int i = 0; i < size; i++)
//    {
//        QString row;
//        for (int j = 0; j < size; j++)
//        {
//            if(wQuadMat[i][j] == DIAG) row += " D ";
//            else row += " " + QString::number(wQuadMat[i][j]) + " ";
//        }
//        row += "  ||  ";
//        for (int j = 0; j < size; j++)
//        {
//            if(bQuadMat[i][j] == DIAG) row += "D ";
//            else row += " " + QString::number(bQuadMat[i][j]) + " ";
//        }
//        qDebug() << row;
//    }
    posMat->printGrid();
    qDebug() << "WHITE: COM=(" << wCom.r << ", " << wCom.c << "), E=" << wE << ", avg=" << wAvgDist << ", #=" << wCount << "Cen=" << wCen << "edge=" << wEdge << "Q=" << wq3q4 << "Area=" << getMinArea(W) << "PSV=" << getPieceSquareValue(W);
    qDebug() << "BLACK: COM=(" << bCom.r << ", " << bCom.c << "), E=" << bE << ", avg=" << bAvgDist << ", #=" << bCount << "Cen=" << bCen << "edge=" << bEdge << "Q=" << bq3q4 << "Area=" << getMinArea(B) << "PSV=" << getPieceSquareValue(B);
}

Grid &BoardState::getGrid()
{
    return *posMat;
}

VC &BoardState::getWhitePositions()
{
    return wPieces;
}

VC &BoardState::getBlackPositions()
{
    return bPieces;
}

float BoardState::getScore(int color)
{
    float wScore = 0, bScore = 0;
    int bpsv = getPieceSquareValue(B);
    int wpsv = getPieceSquareValue(W);
    int bArea = getMinArea(B);
    int wArea = getMinArea(W);
    bScore = bpsv - 10 * bArea + 40 * bCen - 100 * bAvgDist + 10 * bq3q4 - 10 * bE;
//    if (bE > 3) bScore -= 20 * bE - 30 * bCen + 80 * bEdge;
//    if (bAvgDist > 2) bScore -= 20 * bAvgDist + 40 * bEdge;
    wScore = wpsv * 1 - 10 * wArea - 100 * wAvgDist - 10 * wE;
//    if (wE < 3) wScore -= 80 * wE;
//    bScore = bpsv - 40 * bArea - 10 * bAvgDist + 10 * bq3q4 - 20 * bE + 10 * bCen /*- 10 * bEdge * bEdge*/;
//    wScore = wpsv - 4 * wArea/* - 1 * wAvgDist + 1 * wq3q4*/ - 10 * wE + 10 * wCen/* - 1 * wEdge*/;
    if (wE <= 1.5 && terminal(W, wPieces.back().r, wPieces.back().c)) wScore = 10000;
    if (bE <= 1.5 && terminal(B, bPieces.back().r, bPieces.back().c)) bScore = 10000;

    return bScore - wScore;
}

BoardState::~BoardState()
{
    delete posMat;
}
