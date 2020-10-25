#include <iostream>
#ifndef SQUARED_MATRIX_H_
#define SQUARED_MATRIX_H_

class SquaredMatrix 
{
    private:
        int m_size;
        int* m_matrix;
        
    public:
        SquaredMatrix(int size);
        ~SquaredMatrix();
        size_t index(int x, int y) const { return m_size * x + y; }
        void setValue(int x, int y, int value) { m_matrix[index(x, y)] = value; }
        void print();

};

#endif