#include <iostream>
#include "matrix.h"
#include "util.h"

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

int Greedy(Matrix distanceMatrix, int nOfCities)
{
    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int parentValue;
    int neighbourValue;

    int *parentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, &distanceMatrix);
    parentValue = SumOfarray(parentDistancesArray, nOfCities);
    bool done = false;
    while (!done)
    {
        // std::cout << "Current result: " << parentValue << std::endl;

        //// generowanie całego sasiedztwa - to pewnie trzeba użyć do steepest i wtedy już łatwo.
        // Matrix neighbourhood = GenerateWhole_2OPT(currentPermutation, nOfCities);
        bool isBetterResult = false;
        for (int i = 0; i < nOfCities; i++)
        {
            for (int j = 1 + i; j < nOfCities; j++)
            {
                int *neighbourPermutation = new int[nOfCities];
                std::copy(currentPermutation, currentPermutation + nOfCities, neighbourPermutation);
                std::swap(neighbourPermutation[i], neighbourPermutation[j]);
                int swappedIndexes[] = {i, j};
                int *neighbourDistancesArray = GetArrayOfDistancesUsingParentPermutation(neighbourPermutation, parentDistancesArray, &distanceMatrix, swappedIndexes, nOfCities);
                // int *neighbourDistancesArray = GetArrayOfDistances(neighbourPermutation, nOfCities, &distanceMatrix);

                neighbourValue = SumOfarray(neighbourDistancesArray, nOfCities);

                if (neighbourValue < parentValue)
                {
                    delete[] currentPermutation;
                    parentDistancesArray = neighbourDistancesArray;

                    currentPermutation = neighbourPermutation;
                    parentValue = neighbourValue;
                    isBetterResult = true;
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
    }
    delete[] parentDistancesArray;

    std::cout << "Finally Greedy result: " << parentValue << std::endl;
    return parentValue;
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
    
    while (icities < nOfCities-1)
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