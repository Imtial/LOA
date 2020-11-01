#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "constants.h"
class Player;
class Controller
{
private:
    Player * p1;
    Player * p2;
    void requestMove();
public:
    int turn;
    Controller(Player * p1, Player * p2);
    void start();
    void processClickEvent(int x, int y);
    int altTurn();
};

#endif // CONTROLLER_H
