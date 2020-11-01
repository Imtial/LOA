#ifndef PLAYER_H
#define PLAYER_H

#define _HUMAN_   "HUMAN"
#define _AI_      "AI"

class Grid;
class Board;
class QString;

class Player
{
private:
    int color;
protected:
    Grid * posMat;
    Board * board;
    bool isMoveValid(int piece, int r1, int c1, int r2, int c2);
public:
    Player(int color);
    void setBoard(Board * board);
    virtual void move(int row = -1, int col = -1) = 0;
    virtual QString getType() = 0;
    int getColor();
};

#endif // PLAYER_H
