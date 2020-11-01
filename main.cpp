#include <iostream>
#include "matrix.h"
#include "util.h"
#include "algorithms.h"


int main(int argc, char **argv)
{
    std::string filepath = argv[1];
    std::cout << "PLIK: " << filepath << std::endl;

    int nOfCities;
    Matrix distanceMatrix = LoadData(filepath, nOfCities);
    // std::cout << distanceMatrix;
    
    // MeasureTimeOfFunctionInMilliSeconds(1, "Greedy",  Greedy, distanceMatrix, nOfCities);
    // MeasureTimeOfFunctionInMilliSeconds(1, "SimpleHeur", SimpleHeur, distanceMatrix, nOfCities);
    
    int result = Greedy(distanceMatrix, nOfCities);
    std::cout << "Greedy: " << result << std::endl;

    result = SimpleHeur(distanceMatrix, nOfCities);
    std::cout << "SimpleHeur: " << result << std::endl;

    result = Steepest(distanceMatrix, nOfCities);
    std::cout << "Steepest: " << result << std::endl;


}
