#include <iostream>
#include "matrix.h"
#include "util.h"

int Greedy(std::string filepath, int nOfCities)
{
    const int nOfNeighbours = (nOfCities * (nOfCities - 1)) / 2;

    Matrix distanceMatrix = LoadData(filepath = filepath, nOfCities = nOfCities);

    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    // int *parentDistancesArray;
    // int *neighbourPermutation = new int[nOfCities];
    // int *neighbourDistancesArray;
    int parentValue;
    int neighbourValue;

    int *parentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, &distanceMatrix);
    parentValue = SumOfarray(parentDistancesArray, nOfCities);
    bool done = false;
    while (!done)
    {

        std::cout << "Current result: " << parentValue << std::endl;

        //// generowanie całego sasiedztwa - to pewnie trzeba użyć do steepest i wtedy już łatwo.
        // Matrix neighbourhood = GenerateNeighbourhood(currentPermutation, nOfCities);
        bool isBetterResult = false;
        for (int i = 0; i < nOfCities; i++)
        {
            for (int j = 1 + i; j < nOfCities; j++)
            // for (size_t i = 0; i < nOfNeighbours; i++)
            {
                // neighbourPermutation = GenerateNeighbour(currentPermutation, nOfCities, i + 1, swappedIndexes);
                // neighbourPermutation = new int[nOfCities];
                int *neighbourPermutation = new int[nOfCities];
                std::copy(currentPermutation, currentPermutation + nOfCities, neighbourPermutation);
                std::swap(neighbourPermutation[i], neighbourPermutation[j]);
                // int *neighbourDistancesArray = GetArrayOfDistances(neighbourPermutation, nOfCities, &distanceMatrix);
                int swappedIndexes[] = {i, j};
                std::cout << "i: " << i << "\tj: " << j << std::endl;
                std::cout << "currentPermutation" << std::endl;
                PrintVector(currentPermutation, nOfCities);

                std::cout << "neighbourPermutation" << std::endl;
                PrintVector(neighbourPermutation, nOfCities);
                std::cout << "parentDistancesArray" << std::endl;
                PrintVector(parentDistancesArray, nOfCities);
                int *neighbourDistancesArray = GetArrayOfDistancesUsingParentPermutation(neighbourPermutation, parentDistancesArray, &distanceMatrix, swappedIndexes, nOfCities);

                std::cout << "neighbourDistancesArray" << std::endl;
                PrintVector(neighbourDistancesArray, nOfCities);
                neighbourValue = SumOfarray(neighbourDistancesArray, nOfCities);
                std::cout << "neighbourValue: " << neighbourValue << "\tparentValue: " << parentValue << std::endl;
                
                if (neighbourValue < parentValue)
                {
                    delete[] currentPermutation;
                    parentDistancesArray = neighbourDistancesArray;

                    currentPermutation = neighbourPermutation;
                    parentValue = neighbourValue;
                    isBetterResult = true;
                    return 1;
                    break;
                }
                else if (i == nOfCities - 2 && j == nOfCities - 1)
                {
                    done = true;
                }
                isBetterResult = false;
                delete[] neighbourDistancesArray;
            }
            if (isBetterResult)
                break;
        }
        delete[] parentDistancesArray;
    }

    std::cout << "Finally Greedy result: " << parentValue << std::endl;
    return parentValue;
}

int main(int argc, char **argv)
{
    std::string filepath = argv[1];
    int nOfCities = std::stoi(argv[2]);
    int x = std::stoi(argv[3]);
    int y = std::stoi(argv[4]);
    std::cout << "PLIK: " << filepath << std::endl;
    std::cout << "LICZBA MIAST: " << nOfCities << std::endl;

    // wczytanie danych
    Matrix distanceMatrix = LoadData(filepath=filepath, nOfCities=nOfCities);
    // std::cout << distanceMatrix << std::endl;
    std::cout << distanceMatrix.GetValue(x,y) << std::endl;

    //// mierzenie czasu funkcji RandomPermutation(5000)
    // MeasureTimeOfFunctionInMilliSeconds(1, RandomPermutation, 1000000);

    //// testowe losowanie permutacji
    // int* randomPermutation = RandomPermutation(5);
    // PrintVector(randomPermutation, 5);
    // delete [] randomPermutation;

    // MeasureTimeOfFunctionInMilliSeconds(1, Greedy, filepath, nOfCities);
}
