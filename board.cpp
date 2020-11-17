#include "board.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <vector>
#include "bsquare.h"
#include "bmark.h"
#include "bcircle.h"
#include "cell.h"
#include "human.h"
#include "ai.h"
#include "controller.h"
#include "boardstate.h"

Board::Board(QGraphicsScene &scene, Controller * con, int size)
    : size(size), nPieces((size - 2)*2), scene(scene),
      posMat(size), moveMat(size, VB(size, 0)), turn(W)
{
    this->con = con;

    int outerSize = size + 2;

    squares = new BSquare ** [outerSize];

    for (int r = 0; r < size; r++)
    {
        squares[r] = new BSquare * [size];

        for (int c = 0; c < size; c++)
        {
            squares[r][c] = new BSquare(r, c, con);
            scene.addItem(squares[r][c]);
        }
    }

    frLabels = new BMark * [size];
    lrLabels = new BMark * [size];
    fcLabels = new BMark * [size];
    lcLabels = new BMark * [size];
    for (int i = 0; i < size; i++)
    {
        frLabels[i] = new BMark(true, true, i, size);
        scene.addItem(frLabels[i]);
        lrLabels[i] = new BMark(true, false, i, size);
        scene.addItem(lrLabels[i]);
        fcLabels[i] = new BMark(false, true, i, size);
        scene.addItem(fcLabels[i]);
        lcLabels[i] = new BMark(false, false, i, size);
        scene.addItem(lcLabels[i]);
    }

    for (int i = 0; i < nPieces/2; i++)
    {
        BCircle * b1 = new BCircle(i+1, 0, false);
        scene.addItem(b1);
        squares[i+1][0]->addCircle(b1);
//        qDebug() << i+1 << ", " << 0 << " | " << squares[i+1][0]->getCircle()->getRow() << ", " << squares[i+1][0]->getCircle()->getCol();

        BCircle * b2 = new BCircle(i+1, size-1, false);
        scene.addItem(b2);
        squares[i+1][size-1]->addCircle(b2);
//        qDebug() << i+1 << ", " << size-1<< " | " << squares[i+1][size-1]->getCircle()->getRow() << ", " << squares[i+1][size-1]->getCircle()->getCol();

        posMat.set(i+1, 0, B);
        posMat.set(i+1, size-1, B);

        BCircle * w1 = new BCircle(0, i+1, true);
        scene.addItem(w1);
        squares[0][i+1]->addCircle(w1);
//        qDebug() << 0 << ", " << i+1 << " | " << squares[0][i+1]->getCircle()->getRow() << ", " << squares[0][i+1]->getCircle()->getCol();

        BCircle * w2 = new BCircle(size-1, i+1, true);
        scene.addItem(w2);
        squares[size-1][i+1]->addCircle(w2);
//        qDebug() << size-1 << ", " << i+1 << " | " << squares[size-1][i+1]->getCircle()->getRow() << ", " << squares[size-1][i+1]->getCircle()->getCol();

        posMat.set(0, i+1, W);
        posMat.set(size-1, i+1, W);
    }

    this->state = new BoardState(posMat);
}

void Board::movePiece(int row, int col)
{
    posMat.set(row, col, posMat.get(selPiece->getRow(), selPiece->getCol()));
    posMat.set(selPiece->getRow(), selPiece->getCol(), 0);
    squares[selPiece->getRow()][selPiece->getCol()]->addCircle(NULL);
    selPiece->setPos(row, col);
    BCircle * existing = squares[row][col]->getCircle();
    if (existing != NULL)
    {
        scene.removeItem(existing);
        delete existing;
    }
    squares[row][col]->addCircle(selPiece);
    selPiece = NULL;
//    printMatrix();
}

void Board::selectPiece(int row, int col)
{
    moveMat.assign(size, VB(size, 0));
    selPiece = squares[row][col]->getCircle();
    generateMoveMat(posMat.get(row, col), row, col);
}

bool Board::processClickEvent(int row, int col)
{
//    qDebug() << row+1 << ", " << col+1;
    if (posMat.get(row, col) == turn)
    {
        clearHlOptions();
        selectPiece(row, col);
        hlSelection(row, col);
        hlOptions();
//        printMatrix(true);
    }
    else
    {
        if (selPiece != NULL && moveMat[row][col])
        {
            Cell cur(selPiece->getRow(), selPiece->getCol());
            Cell target(row, col);
            int selectedPieceCol = posMat.get(cur);
            int targetPieceCol = posMat.get(target);

            clearHlSelection(cur.r, cur.c);
            movePiece(row, col);
            clearHlOptions();

            state->update(cur, selectedPieceCol, target, targetPieceCol);
            state->print();
            qDebug() << "SCORE=" << state->getScore(turn);
            bool wWin = state->euler(W) <= 1.5 && state->terminal(W, state->getWhitePositions().back().r, state->getWhitePositions().back().c);
            bool bWin = state->euler(B) <= 1.5 && state->terminal(B, state->getBlackPositions().back().r, state->getBlackPositions().back().c);
            if (wWin && bWin)
            {
                if (turn == W) con->end(QString("White"));
                else if (turn == B) con->end(QString("Black"));
            }
            else if (wWin)
                con->end(QString("White"));
            else if (bWin)
                con->end(QString("Black"));
            else
                turn = con->altTurn();
        }
    }

    return true;
}

Grid *Board::getPositionMatrix()
{
    return &posMat;
}

bool Board::isMoveAvailable(int row, int col)
{
    return moveMat[row][col];
}

bool Board::isMoveValid(int piece, const Cell &c1, const Cell &c2)
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

void Board::generateMoveMat(int piece, int row, int col)
{
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
//    qDebug() << "(prow=" << prow << ", pcol" << pcol << "), (nrow=" << nrow << ", ncol=" << ncol << ")";
//    qDebug() << "#row=" << rc << ", #col=" << cc << ", #+diag=" << pdc << ", #-diag=" << ndc;

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

void Board::hlOptions()
{
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (moveMat[r][c]) squares[c][r]->highlight();
        }
    }
}

void Board::hlSelection(int row, int col)
{
    squares[col][row]->highlight(0);
}

void Board::clearHlSelection(int row, int col)
{
    squares[col][row]->clearHighlight();
}

void Board::clearHlOptions()
{
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
//            if (moveMat[r][c])
                squares[c][r]->clearHighlight();
        }
    }
}

void Board::printMatrix(bool printMove)
{
    QString str = "";
    if (!printMove)
    {
        posMat.printGrid();
    }
    else
    {
        qDebug() << "MOVE MATRIX";
        for (VB &row : moveMat)
        {
            for(std::vector<bool>::iterator it = row.begin(); it != row.end(); ++it)
            {
                str += QString::number(*it) + " ";
            }
            qDebug() << str;
            str.clear();
        }
    }
}

Board::~Board()
{
    for (int r = 0; r < size; r++)
    {
        squares[r] = new BSquare * [size];

        for (int c = 0; c < size; c++)
        {
            delete [] squares[r][c];
        }
        delete [] squares[r];
    }
    delete [] squares;

    for (int i = 0; i < size; i++)
    {
        delete [] frLabels[i];
        delete [] lrLabels[i];
        delete [] fcLabels[i];
        delete [] lcLabels[i];
    }
    delete [] frLabels;
    delete [] lrLabels;
    delete [] fcLabels;
    delete [] lcLabels;
}
