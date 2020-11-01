#include "human.h"
#include "board.h"
#include <QString>

Human::Human(int color) : Player(color)
{

}

void Human::move(int row, int col)
{
    this->board->processClickEvent(row, col);
}

QString Human::getType()
{
    return _HUMAN_;
}
