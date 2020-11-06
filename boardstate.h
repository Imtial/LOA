#ifndef BOARDSTATE_H
#define BOARDSTATE_H
#include <cstdint>
#include <list>
#include <vector>
#include "cell.h"

typedef std::vector<int8_t> VI;
typedef std::vector<VI> VVI;
typedef std::list<Cell> LC;

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
    LC wPieces, bPieces;
    int wAvgDist, bAvgDist;
    int8_t wCen, bCen;
    int8_t wEdge, bEdge;
    int quadType(int r, int c, int color);
    float cellScore(int r, int c, int color);
    float updateQuadScore(int r, int c, int color);
    void updateCOM(Cell &init, int initCol, Cell &target, int tarCol);
    void recalculateDistMetrics();
    int getCenScoreChange(Cell &init, int initCol, Cell &target, int tarCol);
    int getCellDist(Cell &c1, Cell &c2);
    bool isEdge(Cell &cell, int d);
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
};

#endif // BOARDSTATE_H
