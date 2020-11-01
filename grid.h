#ifndef GRID_H
#define GRID_H

#include <vector>

typedef std::vector<bool> VB;
typedef std::vector<std::vector<bool>> VVB;

class Grid
{
private:
    VVB pos;
    VVB neg;
public:
    Grid(int size);
    void set(int r, int c, int val);
    int get(int r, int c);
    void printGrid();
};

#endif // GRID_H
