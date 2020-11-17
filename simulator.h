#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <QThread>
#include <cell.h>
#include <chrono>
#include <ctime>
using namespace std::chrono;

class BoardState;
class Grid;

typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;
typedef std::vector<Cell> VC;

class Simulator : public QThread
{
    Q_OBJECT
private:
    BoardState * copiedState;
    Grid& _posMat;
    Cell _from, _to;
    high_resolution_clock::time_point start_t;
    int color;
    int nodeCount;
    bool isMoveValid(int piece, const Cell &c1, const Cell &c2, Grid& posMat);
    void generateMoveList(int piece, int row, int col, VC& moves, Grid& posMat);
    float minimax(int maximizer, BoardState& state, int depth, float alpha, float beta);
public:
    Simulator(BoardState &state, int color);
    ~Simulator();
    void run() override;
signals:
    void squareSelected(int row, int col);
};

#endif // SIMULATOR_H
