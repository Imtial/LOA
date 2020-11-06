#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <QThread>
typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;

class BoardState;
class Cell;
class Grid;
class Simulator : public QThread
{
    Q_OBJECT
private:
    BoardState * copiedState;
    Grid& posMat;
    VVB moveMat;
    int color;
    bool isMoveValid(int piece, const Cell &c1, const Cell &c2);
    void generateMoveMat(int piece, int row, int col);
public:
    Simulator(BoardState &state, int color);
    ~Simulator();
    void run() override;
signals:
    void squareSelected(int row, int col);
};

#endif // SIMULATOR_H
