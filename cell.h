#ifndef CELL_H
#define CELL_H
#include <cstdint>

//class Cell;
//class CellF
//{
//public:
//    float r;
//    float c;

//    CellF();
//    CellF(float row, float col);
//    CellF(const Cell& cell);
//    CellF operator+(float n);
//    CellF operator-(float n);
//    CellF operator*(float n);
//    CellF operator/(float n);
//    CellF operator+(const CellF& rhs);
//    CellF operator-(const CellF& rhs);
//    CellF operator*(const CellF& rhs);
//    CellF operator/(const CellF& rhs);
//    CellF& operator+=(float n);
//    CellF& operator-=(float n);
//    CellF& operator*=(float n);
//    CellF& operator/=(float n);
//    CellF& operator+=(const CellF& rhs);
//    CellF& operator-=(const CellF& rhs);
//    CellF& operator*=(const CellF& rhs);
//    CellF& operator/=(const CellF& rhs);
//    CellF& operator=(const Cell& rhs);
//};

class Cell
{
public:
    int8_t r;
    int8_t c;

    Cell();
    Cell(int8_t row, int8_t col);
//    Cell(const CellF &cellF);
    Cell operator+(int8_t n);
    Cell operator-(int8_t n);
    Cell operator*(int8_t n);
    Cell operator/(int8_t n);
    Cell operator+(const Cell& rhs);
    Cell operator-(const Cell& rhs);
    Cell operator*(const Cell& rhs);
    Cell operator/(const Cell& rhs);
    Cell& operator+=(int8_t n);
    Cell& operator-=(int8_t n);
    Cell& operator*=(int8_t n);
    Cell& operator/=(int8_t n);
    Cell& operator+=(const Cell& rhs);
    Cell& operator-=(const Cell& rhs);
    Cell& operator*=(const Cell& rhs);
    Cell& operator/=(const Cell& rhs);
    bool operator==(const Cell& rhs);
    bool operator!=(const Cell& rhs);
//    Cell& operator=(const CellF& rhs);
};

#endif // CELL_H
