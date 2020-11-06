#ifndef GRID_H
#define GRID_H

#include <vector>

typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;
class Cell;
class Grid
{
private:
    VVB pos;
    VVB neg;
public:
    Grid(int size);
    void set(int r, int c, int val);
    void set(Cell &cell, int val);
    int get(int r, int c);
    int get(Cell &cell);
    void printGrid();
    int size();
};

#endif // GRID_H
