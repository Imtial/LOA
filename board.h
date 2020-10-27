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

typedef std::vector<int> VI;
typedef std::vector<std::vector<int>> VVI;
typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;

#define W   1
#define B   -1
#define ALT(p) -p

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
    VVI posMat;
    VVB moveMat;
    BCircle * selPiece;
    int turn;
    bool isMoveValid(int piece, int r1, int c1, int r2, int c2);
    void hlOptions();
    void clearHlOptions();
    void generateMoveMat(int piece, int row, int col);
    void printMatrix(bool printMove = false);
public:
    Board(QGraphicsScene &scene, int size = SIZE);
    void movePiece(int row, int col);
    void selectPiece(int row, int col);
    bool processClickEvent(int x, int y);
    ~Board();
};

#endif // BOARD_H
