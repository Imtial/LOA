#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "grid.h"

typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;

class QGraphicsScene;
class BCircle;
class BSquare;
class BMark;
class Cell;
class Player;
class Controller;
class BoardState;

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
    Controller * con;
    int turn;
    bool isMoveValid(int piece, const Cell &c1, const Cell &c2);
    void hlOptions();
    void hlSelection(int row, int col);
    void clearHlSelection(int row, int col);
    void clearHlOptions();
    void printMatrix(bool printMove = false);
public:
    BoardState * state;
    Board(QGraphicsScene &scene, Controller * con, int size = SIZE);
    void generateMoveMat(int piece, int row, int col);
    void movePiece(int row, int col);
    void selectPiece(int row, int col);
    bool processClickEvent(int row, int col);
    Grid *getPositionMatrix();
    bool isMoveAvailable(int row, int col);
    ~Board();
};

#endif // BOARD_H
