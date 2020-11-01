#include "controller.h"
#include "player.h"

Controller::Controller(Player * p1, Player * p2) : turn(W)
{
    this->p1 = p1;
    this->p2 = p2;
}

void Controller::start()
{
    if (p1->getType() == _AI_)
    {
        processClickEvent(0, 0);
        if (p2->getType() == _AI_)
        {
            processClickEvent(0, 0);
            int T = 50;
            while (T--) requestMove();
        }
    }
}

void Controller::processClickEvent(int x, int y)
{
    int r, c;
    r = y / DL;
    c = x / DL;
    if (p1->getType() == _HUMAN_ && turn == W)
    {
        p1->move(r-1, c-1);
    }
    else if (p2->getType() == _HUMAN_ && turn == B)
    {
        p2->move(r-1, c-1);
    }
    requestMove();
}

void Controller::requestMove()
{
    if (p1->getType() == _AI_ && turn == W)
    {
        p1->move();
    }
    else if (p2->getType() == _AI_ && turn == B)
    {
        p2->move();
    }
}

int Controller::altTurn()
{
    turn = ALT(turn);
    return turn;
}
