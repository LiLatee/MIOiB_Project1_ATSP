#include <iostream>
#include "matrix.h"
#include "util.cpp"
#include "algorithms_helpers_funcs.cpp"

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

int Greedy(Matrix distanceMatrix, int nOfCities, ResultStruct &resultStruct)
{
    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int *currentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, distanceMatrix);
    int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);
    delete[] currentDistancesArray;
    bool done = false;
    int nOfSteps = 0;
    int nOfAllCheckedResults = 0;
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
                nOfAllCheckedResults++;
                // wholeTime += TimeSinceEpochMillisec() - start;
                if (neighbourValue < currentPermutationValue)
                {
                    std::swap(currentPermutation[i], currentPermutation[j]);
                    currentPermutationValue = neighbourValue;
                    isBetterResult = true;
                    nOfSteps++;
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

    // std::cout << "Finally Greedy result: " << currentPermutationValue << std::endl;
    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.result = currentPermutationValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfAllCheckedResults;
        resultStruct.resultPermutation = std::vector<int>(currentPermutation, currentPermutation + nOfCities);
    }

    delete[] currentPermutation;
    return currentPermutationValue;
}

int Steepest(Matrix distanceMatrix, int nOfCities, ResultStruct &resultStruct)
{
    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int *currentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, distanceMatrix);
    int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);
    delete[] currentDistancesArray;
    bool done = false;
    int nOfSteps = 0;
    int nOfAllCheckedResults = 0;
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
                nOfAllCheckedResults++;
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
            nOfSteps++;
        }
        else
            done = true;
    }
    // std::cout << "wholeTime: " << wholeTime << std::endl;
    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.result = currentPermutationValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfAllCheckedResults;
        resultStruct.resultPermutation = std::vector<int>(currentPermutation, currentPermutation + nOfCities);
    }
    delete[] currentPermutation;
    // std::cout << "Finally Steepest result: " << currentPermutationValue << std::endl;
    return currentPermutationValue;
}

int SimpleHeur(Matrix distanceMatrix, int nOfCities, ResultStruct &resultStruct)
{
    int *finallPath = RandomPermutation(nOfCities);
    int icities = 1;
    int distanceValue = 0;
    int currentCity = finallPath[0];
    int destinationCityIndex = 1;
    int currentDistance = 0;
    int possibleDistance = 0;
    int nOfSteps = 0 ;

    while (icities < nOfCities - 1)
    {
        destinationCityIndex = icities;
        currentDistance = 999999;

        for (int i = icities; i < nOfCities; i++)
        {
            possibleDistance = distanceMatrix.GetValue(currentCity, finallPath[i]);
            if ((currentDistance > possibleDistance) & (possibleDistance > 0))
            {
                currentDistance = possibleDistance;
                destinationCityIndex = i;
            }
            nOfSteps++;
        }

        distanceValue = distanceValue + distanceMatrix.GetValue(currentCity, finallPath[destinationCityIndex]);
        std::swap(finallPath[icities], finallPath[destinationCityIndex]);
        currentCity = finallPath[icities];
        icities++;

    }
    distanceValue = distanceValue + distanceMatrix.GetValue(finallPath[nOfCities - 1], finallPath[0]);


    int sum = 0;
    for (int i = 0; i < nOfCities - 1; i++)
    {
        int cost = distanceMatrix.GetValue(finallPath[i + 1], finallPath[i]);
        sum = sum + cost;
    }

    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.result = distanceValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfSteps;
        resultStruct.resultPermutation = std::vector<int>(finallPath, finallPath + nOfCities);
    }

    return distanceValue;
}

int Random(Matrix distanceMatrix, int nOfCities, int timeInS, ResultStruct &resultStruct)
{
    time_t current_time = time(NULL);

    int bestDistanceValue = 0;
    int *finallPath = RandomPermutation(nOfCities);
    std::vector<int> bestPath;
    int nOfSteps = 0;
    for(int i =0; i<nOfCities-1;i++){
        int cost = distanceMatrix.GetValue( finallPath[i],finallPath[i+1]);
        bestDistanceValue = bestDistanceValue + cost;
    }
    bestDistanceValue= bestDistanceValue+ distanceMatrix.GetValue( finallPath[nOfCities- 1],finallPath[0]);

    while(time(NULL) < current_time+ timeInS){
        int *newPath = RandomPermutation(nOfCities);
        int newDistanceValue =  0;

        for(int i =0; i<nOfCities-1;i++){
            int cost = distanceMatrix.GetValue( newPath[i],newPath[i+1]);
            newDistanceValue = newDistanceValue + cost;
        }
        newDistanceValue = newDistanceValue + distanceMatrix.GetValue( newPath[nOfCities- 1],newPath[0]);

        if(newDistanceValue < bestDistanceValue){
            bestDistanceValue = newDistanceValue;
            bestPath = std::vector<int>(newPath, newPath + nOfCities);
        }
        delete[] newPath;  
        nOfSteps++;
    }

    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.result = bestDistanceValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfSteps;
        resultStruct.resultPermutation = bestPath;
    }
    delete[] finallPath;  
      
    return bestDistanceValue;
}

int RandomWalk(Matrix distanceMatrix, int nOfCities, int timeInS, ResultStruct &resultStruct){

    time_t current_time = time(NULL);

    int bestDistanceValue = 9999999;
    int *finallPath = RandomPermutation(nOfCities);
    int newDistanceValue = 0;
    int oldDistanceValue = 0;
    int nOfSteps = 0;
    std::vector<int> bestPath;

    for(int i =0; i<nOfCities-1;i++){
            int cost = distanceMatrix.GetValue( finallPath[i],finallPath[i+1]);
            oldDistanceValue = oldDistanceValue + cost;
        }
    oldDistanceValue = oldDistanceValue + distanceMatrix.GetValue( finallPath[nOfCities- 1],finallPath[0]);

    while(time(NULL) < current_time+ timeInS){
        Pair swappedIndexes = getRandomNeighbour(finallPath,nOfCities);

        newDistanceValue = oldDistanceValue - getCostDiffForNeighbour(finallPath,distanceMatrix,swappedIndexes,nOfCities); 
        
        if (newDistanceValue < bestDistanceValue){
            bestDistanceValue = newDistanceValue;
            bestPath = std::vector<int>(finallPath, finallPath + nOfCities);
        }
        oldDistanceValue =newDistanceValue;
        nOfSteps++;
    }
    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.result = bestDistanceValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfSteps;
        resultStruct.resultPermutation = bestPath;
    }
    delete[] finallPath;

    return bestDistanceValue;
    
}

#endif