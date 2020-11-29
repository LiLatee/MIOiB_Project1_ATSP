#include <iostream>
#include <cmath>
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
    if (resultStruct.iterationNumber != -1)
        resultStruct.initialResult = currentPermutationValue;
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
        resultStruct.finalResult = currentPermutationValue;
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
    if (resultStruct.iterationNumber != -1)
        resultStruct.initialResult = currentPermutationValue;
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
        resultStruct.finalResult = currentPermutationValue;
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
    int nOfSteps = 0;

    while (icities < nOfCities)
    {
        destinationCityIndex = icities;
        currentDistance = INT32_MAX;

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

    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.finalResult = distanceValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfSteps;
        resultStruct.resultPermutation = std::vector<int>(finallPath, finallPath + nOfCities);
    }

    return distanceValue;
}

int Random(Matrix distanceMatrix, int nOfCities, double timeInMillisec, ResultStruct &resultStruct)
{
    double current_time = TimeSinceEpochMillisec();
    std::cout.precision(17);
    // std::cout << "current_time: " << current_time << std::endl;
    // std::cout << "CZAS: " << timeInS << std::endl;
    // std::cout << "CZAS: " << timeInS+current_time << std::endl;
    // exit(0);
    int bestDistanceValue = 0;
    int *finallPath = RandomPermutation(nOfCities);
    std::vector<int> bestPath;
    int nOfSteps = 0;
    for (int i = 0; i < nOfCities - 1; i++)
    {
        int cost = distanceMatrix.GetValue(finallPath[i], finallPath[i + 1]);
        bestDistanceValue = bestDistanceValue + cost;
    }
    bestDistanceValue = bestDistanceValue + distanceMatrix.GetValue(finallPath[nOfCities - 1], finallPath[0]);
    if (resultStruct.iterationNumber != -1)
        resultStruct.initialResult = bestDistanceValue;
    while (TimeSinceEpochMillisec() < current_time + timeInMillisec)
    {

        int *newPath = RandomPermutation(nOfCities);
        int newDistanceValue = 0;

        for (int i = 0; i < nOfCities - 1; i++)
        {
            int cost = distanceMatrix.GetValue(newPath[i], newPath[i + 1]);
            newDistanceValue = newDistanceValue + cost;
        }
        newDistanceValue = newDistanceValue + distanceMatrix.GetValue(newPath[nOfCities - 1], newPath[0]);

        if (newDistanceValue < bestDistanceValue)
        {
            bestDistanceValue = newDistanceValue;
            bestPath = std::vector<int>(newPath, newPath + nOfCities);
        }
        delete[] newPath;
        nOfSteps++;
        // std::cout << "time(NULL): " << time(NULL) << std::endl;
        // std::cout << "current_time: " << current_time << std::endl;
        // std::cout << "current_time + timeInS: " << timeInS+current_time << std::endl;
        // std::cout << "roznica: " << time(NULL)-current_time << std::endl;
    }

    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.finalResult = bestDistanceValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfSteps;
        if (bestPath.size() == 0)
            resultStruct.resultPermutation = std::vector<int>(finallPath, finallPath + nOfCities);
        else
            resultStruct.resultPermutation = bestPath;
    }
    delete[] finallPath;

    return bestDistanceValue;
}

int RandomWalk(Matrix distanceMatrix, int nOfCities, double timeInMillisec, ResultStruct &resultStruct)
{

    time_t current_time = TimeSinceEpochMillisec();

    int bestDistanceValue = INT32_MAX;
    int *finallPath = RandomPermutation(nOfCities);
    int newDistanceValue = 0;
    int oldDistanceValue = 0;
    int nOfSteps = 0;
    std::vector<int> bestPath;

    for (int i = 0; i < nOfCities - 1; i++)
    {
        int cost = distanceMatrix.GetValue(finallPath[i], finallPath[i + 1]);
        oldDistanceValue = oldDistanceValue + cost;
    }
    oldDistanceValue = oldDistanceValue + distanceMatrix.GetValue(finallPath[nOfCities - 1], finallPath[0]);

    if (resultStruct.iterationNumber != -1)
        resultStruct.initialResult = oldDistanceValue;

    while (TimeSinceEpochMillisec() < current_time + timeInMillisec)
    {
        Pair swappedIndexes = getRandomNeighbour(finallPath, nOfCities);

        newDistanceValue = ComputePossibleValue(finallPath, oldDistanceValue, distanceMatrix, swappedIndexes, nOfCities); //oldDistanceValue - getCostDiffForNeighbour(finallPath, distanceMatrix, swappedIndexes, nOfCities);
        std::swap(finallPath[swappedIndexes.first], finallPath[swappedIndexes.second]);

        if (newDistanceValue < bestDistanceValue)
        {
            bestDistanceValue = newDistanceValue;
            bestPath = std::vector<int>(finallPath, finallPath + nOfCities);
        }
        oldDistanceValue = newDistanceValue;
        nOfSteps++;
    }
    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.finalResult = bestDistanceValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfSteps;
        if (bestPath.size() == 0)
            resultStruct.resultPermutation = std::vector<int>(finallPath, finallPath + nOfCities);
        else
            resultStruct.resultPermutation = bestPath;
    }
    delete[] finallPath;

    return bestDistanceValue;
}

int SW(Matrix distanceMatrix, int nOfCities, ResultStruct &resultStruct)
{
    const int nOfSamples = 1000;
    const int maxNOfstepsWithouCChange = nOfCities * nOfCities * 3;
    const float alpha = 0.99;
    const int maxNOfstepsWithouValueImprovement = 10000;

    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int *currentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, distanceMatrix);
    int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);

    int *bestPermutation = new int[nOfCities];
    std::copy(currentPermutation, currentPermutation + nOfCities, bestPermutation);
    int bestPermutationValue = currentPermutationValue;

    if (resultStruct.iterationNumber != -1)
        resultStruct.initialResult = currentPermutationValue;
    delete[] currentDistancesArray;

    float C = genInitCValue(distanceMatrix, nOfCities, nOfSamples);
    int stepsWthoutCChange = 0;
    bool done = false;
    int nOfSteps = 0;
    int nOfAllCheckedResults = 0;
    int nOfstepsWithouImprovement = 0;
    while (!done)
    {
        // std::cout << "Current result: " << currentPermutationValue << std::endl;

        Pair<int> swappedIndex = getRandomNeighbour(currentPermutation, nOfCities);
        int neighbourValue = ComputePossibleValue(currentPermutation, currentPermutationValue, distanceMatrix, swappedIndex, nOfCities);
        nOfAllCheckedResults++;
        // std::cout << "curr: " << currentPermutationValue << "\tneigh: " << neighbourValue << std::endl;
        if (neighbourValue <= currentPermutationValue)
        {
            // std::cout << "LEPSZY\n";
            std::swap(currentPermutation[swappedIndex[0]], currentPermutation[swappedIndex[1]]);
            currentPermutationValue = neighbourValue;

            nOfstepsWithouImprovement++;
            nOfSteps++;
            stepsWthoutCChange++;

            if (currentPermutationValue < bestPermutationValue)
            {
                std::copy(currentPermutation, currentPermutation + nOfCities, bestPermutation); // TODO
                bestPermutationValue = currentPermutationValue;
                nOfstepsWithouImprovement = 0;
            }


        }
        else if (neighbourValue > currentPermutationValue)
        {
            // std::cout << "GORSZY\n";
            nOfstepsWithouImprovement++;
            float randomNumber = (float)std::rand() / RAND_MAX;
            float prob = std::exp((-neighbourValue - currentPermutationValue) / C);
            if (prob > randomNumber)
            {
                // std::cout << "GORSZY SWAP\n";
                std::swap(currentPermutation[swappedIndex[0]], currentPermutation[swappedIndex[1]]);
                currentPermutationValue = neighbourValue;
                nOfSteps++;
                stepsWthoutCChange++;
            }
        }

        if (nOfstepsWithouImprovement > maxNOfstepsWithouValueImprovement)
            done = true;

        if (stepsWthoutCChange > maxNOfstepsWithouCChange)
        {
            C *= alpha;
            stepsWthoutCChange = 0;
        }
    }
    // std::cout << "wholeTime: " << wholeTime << std::endl;

    // std::cout << "Finally Greedy result: " << currentPermutationValue << std::endl;
    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.finalResult = bestPermutationValue;
        resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfAllCheckedResults;
        resultStruct.resultPermutation = std::vector<int>(bestPermutation, bestPermutation + nOfCities);
    }

    delete[] currentPermutation;
    delete[] bestPermutation;
    return bestPermutationValue;
}

#endif