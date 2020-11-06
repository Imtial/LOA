#include "ai.h"
#include <ctime>
#include <vector>
#include "board.h"
#include "cell.h"
#include "simulator.h"

typedef std::vector<Cell> VC;

AI::AI(int color) : Player(color)
{

}

void AI::move(int row, int col)
{
    Simulator * sim = new Simulator(*board->state, getColor());
    connect(sim, SIGNAL(squareSelected(int, int)), this, SLOT(onSquareSelected(int, int)));
    connect(sim, SIGNAL(finished()), sim, SLOT(deleteLater()));
    sim->start();
}

void AI::onSquareSelected(int row, int col)
{
    board->processClickEvent(row, col);
}

QString AI::getType()
{
    return _AI_;
}
