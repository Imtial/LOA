#include "ai.h"
#include <ctime>
#include <vector>
#include <QEventLoop>
#include <QTimer>
#include "board.h"
#include "cell.h"

typedef std::vector<Cell> VC;

AI::AI(int color) : Player(color)
{

}

void AI::move(int row, int col)
{
    int size = posMat->size();
    VC pos;
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            if (posMat->get(r, c) == getColor()) pos.push_back(Cell(r,c));
    Cell &selected = pos[rand() % pos.size()];
    board->processClickEvent(selected.r, selected.c);

    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(1000);
    loop.exec();

    pos.clear();
    board->generateMoveMat(getColor(), selected.r, selected.c);
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            if (board->isMoveAvailable(r, c)) pos.push_back(Cell(r,c));
    selected = pos[rand() % pos.size()];
    board->processClickEvent(selected.r, selected.c);
}

QString AI::getType()
{
    return _AI_;
}
