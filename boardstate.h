#ifndef BOARDSTATE_H
#define BOARDSTATE_H
#include <cstdint>
#include <list>
#include <vector>
#include "cell.h"

typedef std::vector<int8_t> VI;
typedef std::vector<VI> VVI;
typedef std::vector<Cell> VC;

class Grid;
class BoardState
{
private:
    Grid * posMat;
    VVI wQuadMat;
    VVI bQuadMat;
    Cell wRaw, bRaw;
    Cell wCom, bCom;
    int8_t wCount, bCount;
    float wE, bE;
    VC wPieces, bPieces;
    float wAvgDist, bAvgDist;
    int8_t wCen, bCen;
    int8_t wEdge, bEdge;
    int8_t wq3q4, bq3q4;
    int quadType(int r, int c, int color);
    float cellScore(int r, int c, int color);
    float updateQuads(int r, int c, int color);
    void updateCOM(Cell &init, int initCol, Cell &target, int tarCol);
    void recalculateDistMetrics();
    void getCenScoreChange(Cell &init, int initCol, Cell &target, int tarCol);
    float getCellDist(Cell &c1, Cell &c2);
    bool isEdge(Cell &cell, int d);
    int getPieceSquareValue(int color);
    int getMinArea(int color);
public:
    BoardState(Grid &posMat);
    ~BoardState();
    BoardState(const BoardState &boardState);
    BoardState& operator=(const BoardState &boardState);
    void initCOM();
    void update(Cell &init, int initCol, Cell &target, int tarCol);
    bool terminal(int color, int sr, int sc);
    float euler(int color);
    void print();
    Grid& getGrid();
    VC& getWhitePositions();
    VC& getBlackPositions();
    float getScore(int color);
};

#endif // BOARDSTATE_H
