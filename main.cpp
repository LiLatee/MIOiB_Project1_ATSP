#include <iostream>
#include "squared_matrix.h"
#include "util.h"

int main(int argc, char** argv) {
    std::string filepath = argv[1];
    int n = std::stoi(argv[2]);
    std::cout << "PLIK: " << filepath << std::endl;
    std::cout << "LICZBA MIAST: " << n << std::endl;
    // std::cout << __cplusplus << std::endl;

    SquaredMatrix matrix = loadData(filepath=filepath, n=n);
    matrix.print();

    // MeasureTimeOfFunctionInMilliSeconds(10, RandomPermutation, 5000);

    // int* randomPermutation = RandomPermutation(5);
    // PrintVector(randomPermutation, 5);
    // delete [] randomPermutation

} 
