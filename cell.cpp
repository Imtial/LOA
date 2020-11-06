#include "cell.h"

Cell::Cell()
{
    this->r = 0;
    this->c = 0;
}

Cell::Cell(int8_t row, int8_t col)
{
    this->r = row;
    this->c = col;
}

//Cell::Cell(const CellF &cellF)
//{
//    this->r = cellF.r + 0.5;
//    this->c = cellF.c + 0.5;
//}

Cell Cell::operator+(int8_t n)
{
    return Cell(r+n, c+n);
}

Cell Cell::operator-(int8_t n)
{
    return Cell(r-n, c-n);
}

Cell Cell::operator*(int8_t n)
{
    return Cell(r*n, c*n);
}

Cell Cell::operator/(int8_t n)
{
    return Cell((r + (n / 2)) / n, (c + (n / 2)) / n);
}

Cell Cell::operator+(const Cell &rhs)
{
    return Cell(r+rhs.r, c+rhs.c);
}

Cell Cell::operator-(const Cell &rhs)
{
    return Cell(r-rhs.r, c-rhs.c);
}

Cell Cell::operator*(const Cell &rhs)
{
    return Cell(r*rhs.r, c*rhs.c);
}

Cell Cell::operator/(const Cell &rhs)
{
    return Cell((r + (rhs.r / 2)) / rhs.r, (c + (rhs.c / 2)) / rhs.c);
}

Cell &Cell::operator+=(int8_t n)
{
    this->r += n;
    this->c += n;
    return *this;
}

Cell &Cell::operator-=(int8_t n)
{
    this->r -= n;
    this->c -= n;
    return *this;
}

Cell &Cell::operator*=(int8_t n)
{
    this->r *= n;
    this->c *= n;
    return *this;
}

Cell &Cell::operator/=(int8_t n)
{
    this->r = (this->r + (n / 2)) / n;
    this->c = (this->c + (n / 2)) / n;
    return *this;
}

Cell &Cell::operator+=(const Cell &rhs)
{
    this->r += rhs.r;
    this->c += rhs.c;
    return *this;
}

Cell &Cell::operator-=(const Cell &rhs)
{
    this->r -= rhs.r;
    this->c -= rhs.c;
    return *this;
}

Cell &Cell::operator*=(const Cell &rhs)
{
    this->r *= rhs.r;
    this->c *= rhs.c;
    return *this;
}

Cell &Cell::operator/=(const Cell &rhs)
{
    this->r = (this->r + (rhs.r / 2)) / rhs.r;
    this->c = (this->c + (rhs.c / 2)) / rhs.c;
    return *this;
}

bool Cell::operator==(const Cell &rhs)
{
    return (this->r == rhs.r && this->c == rhs.c);
}

bool Cell::operator!=(const Cell &rhs)
{
    return !(this->r == rhs.r && this->c == rhs.c);
}

//Cell &Cell::operator=(const CellF &rhs)
//{
//    this->r = rhs.r + 0.5;
//    this->c = rhs.c + 0.5;
//    return *this;
//}


//CellF::CellF()
//{
//    this->r = 0;
//    this->c = 0;
//}

//CellF::CellF(float row, float col)
//{
//    this->r = row;
//    this->c = col;
//}

//CellF::CellF(const Cell &cell)
//{
//    this->r = cell.r;
//    this->c = cell.c;
//}

//CellF CellF::operator+(float n)
//{
//    return CellF(r+n, c+n);
//}

//CellF CellF::operator-(float n)
//{
//    return CellF(r-n, c-n);
//}

//CellF CellF::operator*(float n)
//{
//    return CellF(r*n, c*n);
//}

//CellF CellF::operator/(float n)
//{
//    return CellF(r / n, c / n);
//}

//CellF CellF::operator+(const CellF &rhs)
//{
//    return CellF(r+rhs.r, c+rhs.c);
//}

//CellF CellF::operator-(const CellF &rhs)
//{
//    return CellF(r-rhs.r, c-rhs.c);
//}

//CellF CellF::operator*(const CellF &rhs)
//{
//    return CellF(r*rhs.r, c*rhs.c);
//}

//CellF CellF::operator/(const CellF &rhs)
//{
//    return CellF(r / rhs.r, c / rhs.c);
//}

//CellF &CellF::operator+=(float n)
//{
//    this->r += n;
//    this->c += n;
//    return *this;
//}

//CellF &CellF::operator-=(float n)
//{
//    this->r -= n;
//    this->c -= n;
//    return *this;
//}

//CellF &CellF::operator*=(float n)
//{
//    this->r *= n;
//    this->c *= n;
//    return *this;
//}

//CellF &CellF::operator/=(float n)
//{
//    this->r = this->r / n;
//    this->c = this->c / n;
//    return *this;
//}

//CellF &CellF::operator+=(const CellF &rhs)
//{
//    this->r += rhs.r;
//    this->c += rhs.c;
//    return *this;
//}

//CellF &CellF::operator-=(const CellF &rhs)
//{
//    this->r -= rhs.r;
//    this->c -= rhs.c;
//    return *this;
//}

//CellF &CellF::operator*=(const CellF &rhs)
//{
//    this->r *= rhs.r;
//    this->c *= rhs.c;
//    return *this;
//}

//CellF &CellF::operator/=(const CellF &rhs)
//{
//    this->r = this->r  / rhs.r;
//    this->c = this->c / rhs.c;
//    return *this;
//}

//CellF &CellF::operator=(const Cell &rhs)
//{
//    this->r = rhs.r;
//    this->c = rhs.c;
//    return *this;
//}
