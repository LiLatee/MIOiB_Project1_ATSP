#include <iostream>
#include "matrix.h"

Matrix::Matrix(int rows, int cols)
{
    m_rows = rows;
    m_cols = cols;
    m_matrix = new int[m_rows*m_cols];
}

Matrix::~Matrix()
{
   delete [] m_matrix;
}

void Matrix::Print()
{
    using namespace std;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            cout << m_matrix[GetIndex(i, j)] << '\t';
            if (j == m_cols-1) cout << endl;
        }
    }
}

int* Matrix::GetRow(int row)
{
    int* result = new int[m_cols];
    std::copy(&m_matrix[row*m_cols], &m_matrix[row*m_cols+m_cols], result);
    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    using namespace std;

    for (int i = 0; i < matrix.GetNOfRows(); i++)
    {
        for (int j = 0; j < matrix.GetNOfCols(); j++)
        {
            
            os << matrix.GetValue(i, j) << '\t';
            if (j == matrix.GetNOfCols()-1) os << endl;
        }
    }
    return os;
}


