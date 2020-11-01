#ifndef PLAYER_H
#define PLAYER_H

class Grid;

class Player
{
    int color;
    Grid &posMat;
    bool isMoveValid(int piece, int r1, int c1, int r2, int c2);
public:
    Player(int color, Grid &posMat);
    int getColor();
};

#endif // PLAYER_H
