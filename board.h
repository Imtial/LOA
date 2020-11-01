#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <vector>
#include "bsquare.h"
#include "bmark.h"
#include "bcircle.h"
#include "grid.h"

class Board
{
    int size;
    int nPieces;
    QGraphicsScene &scene;
    BMark ** frLabels;
    BMark ** lrLabels;
    BMark ** fcLabels;
    BMark ** lcLabels;
    BSquare *** squares;
    Grid posMat;
    VVB moveMat;
    BCircle * selPiece;
    int turn;
    bool isMoveValid(int piece, int r1, int c1, int r2, int c2);
    void hlOptions();
    void clearHlOptions();
    void printMatrix(bool printMove = false);
public:
    Board(QGraphicsScene &scene, int size = SIZE);
    void generateMoveMat(int piece, int row, int col);
    void movePiece(int row, int col);
    void selectPiece(int row, int col);
    bool processClickEvent(int x, int y);
    ~Board();
};

#endif // BOARD_H
