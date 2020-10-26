#include <iostream>
#ifndef MATRIX_H_
#define MATRIX_H_

class Matrix
{
private:
    int m_rows;
    int m_cols;
    int *m_matrix;

public:
    Matrix(int rows, int cols);
    ~Matrix();
    size_t GetIndex(int row, int column) const { return m_rows * row + column; }
    int GetValue(int row, int column) const { return m_matrix[GetIndex(row, column)]; }
    int GetNOfRows() const { return m_rows; }
    int GetNOfCols() const { return m_cols; }
    void SetValue(int row, int column, int value) { m_matrix[GetIndex(row, column)] = value; }
    int *GetRow(int row);
    void SetRow(int row, int arr[]) { std::copy(arr, arr + m_cols, m_matrix + row * m_cols); }
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
    void Print();
};

#endif