#include <iostream>
#include "matrix.h"
#include "util.h"

int Greedy(std::string filepath, int nOfCities)
{
    const int nOfNoImprovementIterations = 10;
    const int nOfNeighbours = (nOfCities * (nOfCities - 1)) / 2;

    Matrix distanceMatrix = LoadData(filepath = filepath, nOfCities = nOfCities);

    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int currentValue;

    bool isBetterResult = true;
    while (isBetterResult)
    {
        int *distancesArrayOfPermutation = GetArrayOfDistances(currentPermutation, nOfCities, &distanceMatrix);
        currentValue = SumOfarray(distancesArrayOfPermutation, nOfCities - 1);
        std::cout << "Current result: " << currentValue << std::endl;

        //// generowanie całego sasiedztwa - to pewnie trzeba użyć do steepest i wtedy już łatwo.
        // Matrix neighbourhood = GenerateNeighbourhood(currentPermutation, nOfCities);

        for (size_t i = 0; i < nOfNeighbours; i++)
        {   
            int *neighbourPermutation = GenerateNeighbour(currentPermutation, nOfCities, i + 1);
            int *neighbourDistancesArrayOfPermutation = GetArrayOfDistances(neighbourPermutation, nOfCities, &distanceMatrix);
            int neighbourValue = SumOfarray(neighbourDistancesArrayOfPermutation, nOfCities - 1);

            if (neighbourValue < currentValue)
            {
                currentPermutation = neighbourPermutation;
                currentValue = neighbourValue;
                delete[] neighbourDistancesArrayOfPermutation;
                break;
            }
            else if (i == nOfNeighbours - 1)
                isBetterResult = false;

            delete[] neighbourDistancesArrayOfPermutation;
            delete[] neighbourPermutation;
        }

        delete[] distancesArrayOfPermutation;
    }
    delete[] currentPermutation;

    std::cout << "Finally Greedy result: " << currentValue << std::endl;
    return currentValue;
}

int main(int argc, char **argv)
{
    std::string filepath = argv[1];
    int nOfCities = std::stoi(argv[2]);
    std::cout << "PLIK: " << filepath << std::endl;
    std::cout << "LICZBA MIAST: " << nOfCities << std::endl;

    //// wczytanie danych
    // Matrix distanceMatrix = LoadData(filepath=filepath, nOfCities=nOfCities);
    // std::cout << distanceMatrix;

    //// mierzenie czasu funkcji RandomPermutation(5000)
    // MeasureTimeOfFunctionInMilliSeconds(1, RandomPermutation, 1000000);

    //// testowe losowanie permutacji
    // int* randomPermutation = RandomPermutation(5);
    // PrintVector(randomPermutation, 5);
    // delete [] randomPermutation;

    MeasureTimeOfFunctionInMilliSeconds(1, Greedy, filepath, nOfCities);

}
