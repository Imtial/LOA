#include "ai.h"
#include "board.h"

//AI::AI(Board &board, VVI &posMat, VVB &moveMat)
//    : board(board), posMat(posMat), moveMat(moveMat)
//{

//}

//PII AI::move()
//{
//    int size = posMat.size();
//    VPII pos;
//    for (int r = 0; r < size; r++)
//        for (int c = 0; c < size; c++)
//            if (posMat[r][c] == B) pos.push_back(qMakePair(r,c));
//    PII selected = pos[rand() % pos.size()];
//    pos.clear();
//    board.generateMoveMat(B, selected.first, selected.second);
//    for (int r = 0; r < size; r++)
//        for (int c = 0; c < size; c++)
//            if (moveMat[r][c]) pos.push_back(qMakePair(r,c));
//    return pos[rand() % pos.size()];;
//}

AI::AI(int color, Grid &posMat) : Player(color, posMat)
{

}
