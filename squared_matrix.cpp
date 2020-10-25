#include <iostream>
#include "squared_matrix.h"

SquaredMatrix::SquaredMatrix(int size)
{
    m_size = size;
    m_matrix = new int[size*size];
}

SquaredMatrix::~SquaredMatrix()
{
   delete [] m_matrix;
}

void SquaredMatrix::print()
{
    using namespace std;

    for (int i = 0; i < m_size; i++)
    {
        for (int j = 0; j < m_size; j++)
        {
            cout << m_matrix[index(i, j)] << '\t';
            if (j == m_size-1) cout << endl;
        }

    }
}

