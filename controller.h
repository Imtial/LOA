#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "constants.h"
#include <QMutex>

class Player;
class Controller : public QObject
{
    Q_OBJECT
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
    void end(QString winner);
    void processClickEvent(int x, int y);
    int altTurn();
signals:
    void foundWinner(QString& winner);
};

#endif // CONTROLLER_H
