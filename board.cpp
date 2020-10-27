#include "board.h"

Board::Board(QGraphicsScene &scene, int size)
    : size(size), nPieces((size - 2)*2), scene(scene),
      posMat(size, VI(size, 0)), moveMat(size, VB(size, 0)), turn(W)
{
    int outerSize = size + 2;

    squares = new BSquare ** [outerSize];

    for (int r = 0; r < size; r++)
    {
        squares[r] = new BSquare * [size];

        for (int c = 0; c < size; c++)
        {
            squares[r][c] = new BSquare(r, c, this);
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

        posMat[i+1][0] = posMat[i+1][size-1] = B;

        BCircle * w1 = new BCircle(0, i+1, true);
        scene.addItem(w1);
        squares[0][i+1]->addCircle(w1);
//        qDebug() << 0 << ", " << i+1 << " | " << squares[0][i+1]->getCircle()->getRow() << ", " << squares[0][i+1]->getCircle()->getCol();

        BCircle * w2 = new BCircle(size-1, i+1, true);
        scene.addItem(w2);
        squares[size-1][i+1]->addCircle(w2);
//        qDebug() << size-1 << ", " << i+1 << " | " << squares[size-1][i+1]->getCircle()->getRow() << ", " << squares[size-1][i+1]->getCircle()->getCol();

        posMat[0][i+1] = posMat[size-1][i+1] = W;
    }

//    printMatrix();
//    for (int r = 0; r < size; r++)
//    {
//        for (int c = 0; c < size; c++)
//        {
//            if (squares[r][c]->getCircle() != NULL)
//                qDebug() << r << ", " << c << " | " << squares[r][c]->getCircle()->getRow() << ", " << squares[r][c]->getCircle()->getCol();
//        }
    //    }
}

void Board::movePiece(int row, int col)
{
    posMat[row][col] = posMat[selPiece->getRow()][selPiece->getCol()];
    posMat[selPiece->getRow()][selPiece->getCol()] = 0;
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
    clearHlOptions();
    moveMat.assign(size, VB(size, 0));
    selPiece = squares[row][col]->getCircle();
    generateMoveMat(posMat[row][col], row, col);
    hlOptions();
}

bool Board::processClickEvent(int x, int y)
{
    int r, c;
    r = y / DL;
    c = x / DL;
    if (posMat[r-1][c-1] == turn)
    {
        selectPiece(r-1, c-1);
//        printMatrix(true);
    }
    else
    {
        if (selPiece != NULL && moveMat[r-1][c-1])
        {
            movePiece(r-1, c-1);
            clearHlOptions();
            turn = ALT(turn);
        }
    }
    qDebug() << r << ", " << c;
    return true;
}

bool Board::isMoveValid(int piece, int r1, int c1, int r2, int c2)
{
    int self = piece;
    int op = ALT(piece);

    if (posMat[r2][c2] == self) return false;

    if (r1 == r2)
    {
        if (c1 < c2)
        {
            for (int i = c1+1; i < c2; i++)
                if (posMat[r1][i] == op) return false;
        }
        else
        {
            for (int i = c1-1; i > c2; i--)
                if (posMat[r1][i] == op) return false;
        }
    }
    else if (c1 == c2)
    {
        if (r1 < r2)
        {
            for (int i = r1+1; i < r2; i++)
                if (posMat[i][c1] == op) return false;
        }
        else
        {
            for (int i = r1-1; i > r2; i--)
                if (posMat[i][c1] == op) return false;
        }
    }
    else if (r1 > r2 && c1 < c2)
    {
        for (int i = r1-1, j = c1+1; i > r2 && j < c2; i--, j++)
            if (posMat[i][j] == op) return false;
    }
    else if (r1 < r2 && c1 > c2)
    {
        for (int i = r1+1, j = c1-1; i < r2 && j > c2; i++, j--)
            if (posMat[i][j] == op) return false;
    }
    else if (r1 > r2 && c1 > c2)
    {
        for (int i = r1-1, j = c1-1; i > r2 && j > c2; i--, j--)
            if (posMat[i][j] == op) return false;
    }
    else if (r1 < r2 && c1 < c2)
    {
        for (int i = r1+1, j = c1+1; i < r2 && j < c2; i++, j++)
            if (posMat[i][j] == op) return false;
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
        if (posMat[row][i]) rc++;
        if (posMat[i][col]) cc++;
        if (prow + i < size && pcol - i >= 0 && posMat[prow + i][pcol - i]) pdc++;
        if (nrow + i < size && ncol + i < size && posMat[nrow + i][ncol + i]) ndc++;
    }
//    qDebug() << "(prow=" << prow << ", pcol" << pcol << "), (nrow=" << nrow << ", ncol=" << ncol << ")";
//    qDebug() << "#row=" << rc << ", #col=" << cc << ", #+diag=" << pdc << ", #-diag=" << ndc;

    if (col + rc < size && isMoveValid(piece, row, col, row, col+rc))
        moveMat[row][col + rc] = true;

    if (col - rc >= 0 && isMoveValid(piece, row, col, row, col-rc))
        moveMat[row][col - rc] = true;

    if (row + cc < size && isMoveValid(piece, row, col, row+cc, col))
        moveMat[row + cc][col] = true;

    if (row - cc >= 0 && isMoveValid(piece, row, col, row-cc, col))
        moveMat[row - cc][col] = true;

    if (row + pdc < size && col - pdc >= 0 && isMoveValid(piece, row, col, row+pdc, col-pdc))
        moveMat[row + pdc][col - pdc] = true;

    if (row - pdc >= 0 && col + pdc < size && isMoveValid(piece, row, col, row-pdc, col+pdc))
        moveMat[row - pdc][col + pdc] = true;

    if (row + ndc < size && col + ndc < size && isMoveValid(piece, row, col, row+ndc, col+ndc))
        moveMat[row + ndc][col + ndc] = true;

    if (row - ndc >= 0 && col - ndc >= 0 && isMoveValid(piece, row, col, row-ndc, col-ndc))
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

void Board::clearHlOptions()
{
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (moveMat[r][c]) squares[c][r]->clearHighlight();
        }
    }
}

void Board::printMatrix(bool printMove)
{
    QString str = "";
    if (!printMove)
    {
        for (VI &row : posMat)
        {
            for(int &val : row)
            {
                str += QString::number(val) + " ";
            }
            qDebug() << str;
            str.clear();
        }
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
