#include "simulator.h"
#include <QEventLoop>
#include <QTimer>
#include <random>
#include <algorithm>
#include <cmath>
#include <boardstate.h>
#include <constants.h>
#include <grid.h>

#define MAX 100000
#define MIN -100000
//typedef std::vector<Cell> VC;

Simulator::Simulator(BoardState &state, int color) : _posMat(state.getGrid()),
    _from(-1, -1), _to(-1, -1), color(color), nodeCount(0)
{
    copiedState = new BoardState(state);
}

Simulator::~Simulator()
{
    delete copiedState;
}

bool Simulator::isMoveValid(int piece, const Cell &c1, const Cell &c2, Grid& posMat)
{
    int self = piece;
    int op = ALT(piece);

    if (posMat.get(c2.r, c2.c) == self) return false;

    if (c1.r == c2.r)
    {
        if (c1.c < c2.c)
        {
            for (int i = c1.c+1; i < c2.c; i++)
                if (posMat.get(c1.r, i) == op) return false;
        }
        else
        {
            for (int i = c1.c-1; i > c2.c; i--)
                if (posMat.get(c1.r, i) == op) return false;
        }
    }
    else if (c1.c == c2.c)
    {
        if (c1.r < c2.r)
        {
            for (int i = c1.r+1; i < c2.r; i++)
                if (posMat.get(i, c1.c) == op) return false;
        }
        else
        {
            for (int i = c1.r-1; i > c2.r; i--)
                if (posMat.get(i, c1.c) == op) return false;
        }
    }
    else if (c1.r > c2.r && c1.c < c2.c)
    {
        for (int i = c1.r-1, j = c1.c+1; i > c2.r && j < c2.c; i--, j++)
            if (posMat.get(i, j) == op) return false;
    }
    else if (c1.r < c2.r && c1.c > c2.c)
    {
        for (int i = c1.r+1, j = c1.c-1; i < c2.r && j > c2.c; i++, j--)
            if (posMat.get(i, j) == op) return false;
    }
    else if (c1.r > c2.r && c1.c > c2.c)
    {
        for (int i = c1.r-1, j = c1.c-1; i > c2.r && j > c2.c; i--, j--)
            if (posMat.get(i, j) == op) return false;
    }
    else if (c1.r < c2.r && c1.c < c2.c)
    {
        for (int i = c1.r+1, j = c1.c+1; i < c2.r && j < c2.c; i++, j++)
            if (posMat.get(i, j) == op) return false;
    }
    return true;
}

void Simulator::generateMoveList(int piece, int row, int col, VC& moves, Grid& posMat)
{
    int size = posMat.size();
    int rc = 0, cc = 0, pdc = 0, ndc = 0;
    int pcol, prow, ncol, nrow;

    if (col + row < size)
    {
        pcol = col + row;
        prow = 0;
    }
    else
    {
        pcol = size - 1;
        prow = row - (size - 1 - col);
    }

    if (col - row >= 0)
    {
        ncol = col - row;
        nrow = 0;
    }
    else
    {
        ncol = 0;
        nrow = row - col;
    }

    for (int i = 0; i < size; i++)
    {
        if (posMat.get(row, i)) rc++;
        if (posMat.get(i, col)) cc++;
        if (prow + i < size && pcol - i >= 0 && posMat.get(prow + i, pcol - i)) pdc++;
        if (nrow + i < size && ncol + i < size && posMat.get(nrow + i, ncol + i)) ndc++;
    }

    Cell curCell(row, col);

    if (col + rc < size && isMoveValid(piece, curCell, Cell(row, col+rc), posMat))
        moves.push_back(Cell(row, col + rc));

    if (col - rc >= 0 && isMoveValid(piece, curCell, Cell(row, col-rc), posMat))
        moves.push_back(Cell(row, col - rc));

    if (row + cc < size && isMoveValid(piece, curCell, Cell(row+cc, col), posMat))
        moves.push_back(Cell(row + cc, col));

    if (row - cc >= 0 && isMoveValid(piece, curCell, Cell(row-cc, col), posMat))
        moves.push_back(Cell(row - cc, col));

    if (row + pdc < size && col - pdc >= 0 && isMoveValid(piece, curCell, Cell(row+pdc, col-pdc), posMat))
        moves.push_back(Cell(row + pdc, col - pdc));

    if (row - pdc >= 0 && col + pdc < size && isMoveValid(piece, curCell, Cell(row-pdc, col+pdc), posMat))
        moves.push_back(Cell(row - pdc, col + pdc));

    if (row + ndc < size && col + ndc < size && isMoveValid(piece, curCell, Cell(row+ndc, col+ndc), posMat))
        moves.push_back(Cell(row + ndc, col + ndc));

    if (row - ndc >= 0 && col - ndc >= 0 && isMoveValid(piece, curCell, Cell(row-ndc, col-ndc), posMat))
        moves.push_back(Cell(row - ndc, col - ndc));
}

float Simulator::minimax(int maximizer, BoardState &state, int depth, float alpha, float beta)
{
    if (depth == DEPTH-1)
        return state.getScore(-maximizer);
    VC &pieces = (maximizer == W) ? state.getWhitePositions() : state.getBlackPositions();
    bool wWin = state.euler(W) <= 1 && state.terminal(W, state.getWhitePositions().back().r, state.getWhitePositions().back().c);
    bool bWin = state.euler(B) <= 1 && state.terminal(B, state.getBlackPositions().back().r, state.getBlackPositions().back().c);

    if (wWin) {
        if (-maximizer == W) return MAX;
        else return MIN;
    }
    if (bWin) {
        if (-maximizer == B) return MAX;
        else return MIN;
    }
    nodeCount++;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    if (maximizer == color)
    {
        float best = MIN;

        std::shuffle(pieces.begin(), pieces.end(), e);
        bool prune = false;
        for (Cell& from : pieces)
        {
            VC moves;
            generateMoveList(maximizer, from.r, from.c, moves, state.getGrid());
//            std::shuffle(moves.begin(), moves.end(), e);
            for (Cell& to : moves)
            {
                BoardState copy = state;
                copy.update(from, maximizer, to, copy.getGrid().get(to.r, to.c));
                float val = minimax(ALT(maximizer), copy, depth+1, alpha, beta);

                best = std::max(best, val);
                alpha = std::max(alpha, best);

                if ((duration_cast<seconds>(high_resolution_clock::now() - start_t)).count() > LIMIT)
                    return best;

                if (alpha < beta)
                {
                    prune = true;
                    break;
                }
            }
            if (prune) break;
        }
        return best;
    }
    else
    {
        float best = MAX;

        bool prune = false;
        std::shuffle(pieces.begin(), pieces.end(), e);
        for (Cell& from : pieces)
        {
            VC moves;
            generateMoveList(maximizer, from.r, from.c, moves, state.getGrid());
//            std::shuffle(moves.begin(), moves.end(), e);
            for (Cell& to : moves)
            {
                BoardState copy = state;
                copy.update(from, maximizer, to, copy.getGrid().get(to.r, to.c));
                float val = minimax(ALT(maximizer), copy, depth+1, alpha, beta);

                best = std::min(best, val);
                beta = std::min(beta, best);

                if ((duration_cast<seconds>(high_resolution_clock::now() - start_t)).count() > LIMIT)
                    return best;

                if (beta <= alpha)
                {
                    prune = true;
                    break;
                }
            }
            if (prune) break;
        }
        return best;
    }
}

void Simulator::run()
{
    float best = MIN;

    VC &pieces = (color == W) ? copiedState->getWhitePositions()
                              : copiedState->getBlackPositions();
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
//    std::shuffle(pieces.begin(), pieces.end(), e);

    start_t = high_resolution_clock::now();

    for (Cell& from : pieces)
    {
        VC moves;
        generateMoveList(color, from.r, from.c, moves, copiedState->getGrid());
//        std::shuffle(moves.begin(), moves.end(), e);
        for (Cell& to : moves)
        {
            BoardState copy = *copiedState;
            copy.update(from, color, to, copy.getGrid().get(to.r, to.c));
            float val = minimax(ALT(color), copy, 0, MIN, MAX);

            if (val >= best)
            {
                _from = from;
                _to = to;
                best = val;
            }

            if ((duration_cast<seconds>(high_resolution_clock::now() - start_t)).count() > LIMIT)
                break;
        }
    }
    Cell neg(-1, -1);

    if (_from == neg || _to == neg)
    {
        qDebug() << "NOT UPDATED.";
        return;
    }
    if (_posMat.get(_from) != color) qDebug() << "WRONG";
    qDebug() << "SCORER=" << best << "VISITED:" << nodeCount << "[" << _from.r << ", " << _from.c << "]--" << "->[" << _to.r << ", " << _to.c << "]";
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_t);
    qDebug() << "THINKING TIME:" << QString::number(duration.count()) << "ms";
    emit squareSelected(_from.r, _from.c);

    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(500);
    loop.exec();

    emit squareSelected(_to.r, _to.c);
}
