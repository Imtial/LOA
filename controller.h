#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "constants.h"
#include <QMutex>

class Player;
class Controller
{
private:
    Player * p1;
    Player * p2;
    mutable QMutex mutex;
    void requestMove();
public:
    int turn;
    bool gameOver;
    Controller(Player * p1, Player * p2);
    void start();
    void processClickEvent(int x, int y);
    int altTurn();
};

#endif // CONTROLLER_H
