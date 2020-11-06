#ifndef AI_H
#define AI_H

#include "player.h"
#include <QObject>

class AI : public QObject, public Player
{
    Q_OBJECT
public:
    AI(int color);
    QString getType() override;
    void move(int row, int col) override;
public slots:
    void onSquareSelected(int row, int col);
signals:
    void moveSignal(int row, int col);
};

#endif // AI_H
