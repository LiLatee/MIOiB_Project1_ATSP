#include <iostream>
#include "matrix.h"
#include "util.h"

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

int Greedy(Matrix distanceMatrix, int nOfCities)
{
    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int *currentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, distanceMatrix);
    int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);
    delete[] currentDistancesArray;
    bool done = false;
    // int wholeTime = 0;
    while (!done)
    {
        // std::cout << "Current result: " << currentPermutationValue << std::endl;
        bool isBetterResult = false;
        for (int i = 0; i < nOfCities; i++)
        {
            for (int j = 1 + i; j < nOfCities; j++)
            {
                // int start = TimeSinceEpochMillisec();
                int neighbourValue = ComputePossibleValue(currentPermutation, currentPermutationValue, distanceMatrix, Pair<int>(i, j), nOfCities);
                // wholeTime += TimeSinceEpochMillisec() - start;
                if (neighbourValue < currentPermutationValue)
                {
                    std::swap(currentPermutation[i], currentPermutation[j]);
                    currentPermutationValue = neighbourValue;
                    isBetterResult = true;
                    break;
                }
                else if (i == nOfCities - 2 && j == nOfCities - 1)
                    done = true;

                isBetterResult = false;
            }
            if (isBetterResult)
                break;
        }
    }
    // std::cout << "wholeTime: " << wholeTime << std::endl;
    delete[] currentPermutation;
    // std::cout << "Finally Greedy result: " << parentValue << std::endl;
    return currentPermutationValue;
}

int Steepest(Matrix distanceMatrix, int nOfCities)
{
    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int *currentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, distanceMatrix);
    int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);
    delete[] currentDistancesArray;
    bool done = false;
    // int wholeTime = 0;
    while (!done)
    {
        // std::cout << "Current result: " << currentPermutationValue << std::endl;
        int currentBestInNeighbourhoodValue = INT32_MAX;
        Pair<int> bestNeighbourSwap;
        bool isChangedPermutation = false;
        for (int i = 0; i < nOfCities; i++)
        {
            for (int j = 1 + i; j < nOfCities; j++)
            {
                Pair<int> swappedIndexes = Pair<int>(i, j);
                // int start = TimeSinceEpochMillisec();
                int neighbourValue = ComputePossibleValue(currentPermutation, currentPermutationValue, distanceMatrix, swappedIndexes, nOfCities);
                // wholeTime += TimeSinceEpochMillisec() - start;
                if (neighbourValue < currentPermutationValue && neighbourValue < currentBestInNeighbourhoodValue)
                {
                    currentBestInNeighbourhoodValue = neighbourValue;
                    bestNeighbourSwap = swappedIndexes;
                    isChangedPermutation = true;
                }
            }
        }

        if (currentBestInNeighbourhoodValue < currentPermutationValue && isChangedPermutation)
        {
            std::swap(currentPermutation[bestNeighbourSwap.first], currentPermutation[bestNeighbourSwap.second]);
            currentPermutationValue = currentBestInNeighbourhoodValue;
        }
        else
            done = true;
    }
    // std::cout << "wholeTime: " << wholeTime << std::endl;
    delete[] currentPermutation;
    // std::cout << "Finally Steepest result: " << currentPermutationValue << std::endl;
    return currentPermutationValue;
}

int SimpleHeur(Matrix distanceMatrix, int nOfCities)
{
    int *finallPath = RandomPermutation(nOfCities);
    int icities = 1;
    int distanceValue = 0;
    int currentCity = finallPath[0];
    int destinationCity = finallPath[1];
    int currentDistance = distanceMatrix.GetValue(currentCity, destinationCity);
    int possibleDistance = 0;

    while (icities < nOfCities - 1)
    {
        for (int i = icities; i < nOfCities; i++)
        {
            // sprawdzamy odległość do miasta
            possibleDistance = distanceMatrix.GetValue(currentCity, finallPath[i]);
            if (currentDistance > possibleDistance)
            {
                currentDistance = possibleDistance;
                destinationCity = i; // GDZIES TUTAJ BŁAD
            }
            // std::cout<<"for"<<i<<std::endl;
        }

        // std::cout<<"distance: "<<currentDistance<<std::endl;
        distanceValue = distanceValue + currentDistance;
        std::swap(finallPath[icities], finallPath[destinationCity]);
        icities++;
        currentCity = destinationCity;
        destinationCity = finallPath[icities];
        currentDistance = distanceMatrix.GetValue(currentCity, destinationCity);
        // std::cout<<"while"<<icities<<std::endl;
    }

    // for(int i=1;i<nOfCities;i++){
    //     std::cout<<finallPath[i]<<std::endl;
    //     std::cout<<"DISTANCE "<<distanceMatrix.GetValue(i,i-1)<<std::endl;

    // }
    std::cout << "Finally SimpleHeur result: " << distanceValue << std::endl;
    return distanceValue;
}

#endif