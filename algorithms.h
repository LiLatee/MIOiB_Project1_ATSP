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
    const int nOfSamples = 10000;
    const int L = (nOfCities * (nOfCities - 1)) / 2;
    const float alpha = 0.9;
    const int P = 10;
    const int maxNOfstepsWithoutValueImprovement = L * P;
    const float C0 = 0.95;
    const float minProbForChange = 0.001;

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

    float maxDiff = genMaxDiffValueInSamples(distanceMatrix, nOfCities, nOfSamples);
    float C = getInitCValue(C0, maxDiff);
    // std::cout << "C: " << C << std::endl;
    int stepsWthoutCChange = 0;
    int nOfstepsWithoutImprovement = 0;

    bool done = false;
    int nOfSteps = 0;
    int nOfAllCheckedResults = 0;
    while (!done)
    {
        Pair<int> swappedIndex = getRandomNeighbour(currentPermutation, nOfCities); // TODO
        int neighbourValue = ComputePossibleValue(currentPermutation, currentPermutationValue, distanceMatrix, swappedIndex, nOfCities);
        nOfAllCheckedResults++;

        if (neighbourValue <= currentPermutationValue)
        {
            std::swap(currentPermutation[swappedIndex[0]], currentPermutation[swappedIndex[1]]);
            currentPermutationValue = neighbourValue;

            if ((resultStruct.iterationNumber != -1) && (currentPermutationValue < bestPermutationValue))
            {
                std::copy(currentPermutation, currentPermutation + nOfCities, bestPermutation); // TODO
            }
            // valueChange = bestPermutationValue - currentPermutationValue;
            if (currentPermutationValue < bestPermutationValue)
            {
                bestPermutationValue = currentPermutationValue;
                nOfstepsWithoutImprovement = 0;
            }
        }
        else if (neighbourValue > currentPermutationValue)
        {
            float randomNumber = (float)std::rand() / RAND_MAX;
            float prob = std::exp(-(neighbourValue - currentPermutationValue) / C);
            if (prob > randomNumber)
            {
                std::swap(currentPermutation[swappedIndex[0]], currentPermutation[swappedIndex[1]]);
                currentPermutationValue = neighbourValue;
            }
            if (prob < minProbForChange && nOfstepsWithoutImprovement > maxNOfstepsWithoutValueImprovement)
                done = true;
        }
        nOfSteps++;
        stepsWthoutCChange++;
        nOfstepsWithoutImprovement++;
        // if (nOfstepsWithoutImprovement > maxNOfstepsWithoutValueImprovement)
        //     done = true;

        if (stepsWthoutCChange > L)
        {
            C *= alpha;
            stepsWthoutCChange = 0;
        }

        // if (valueChange < 0)
        //     valueChange = 0;
        // avgValueChange = (avgValueChange * (nOfSteps) + valueChange) / nOfSteps;
        // std::cout << "maxDiff/100: " << maxDiff / 100 << std::endl;
        // std::cout << "nOfSteps: " << nOfSteps << std::endl;
        // std::cout << "avgValueChange: " << avgValueChange << std::endl;
        // if (avgValueChange < maxDiff / 100)
        // {
        //     done = true;
        // }
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

int SW2(Matrix distanceMatrix, int nOfCities, ResultStruct &resultStruct)
{
    const int nOfSamples = 10000;
    const int maxNOfstepsWithouCChange = (nOfCities * nOfCities) / 2;
    const float alpha = 0.99;
    const int maxNOfstepsWithouImprovement = 100000;

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

    float maxDiff = genMaxDiffValueInSamples(distanceMatrix, nOfCities, nOfSamples);
    float C = getInitCValue(alpha, maxDiff);
    int stepsWthoutCChange = 0;
    int nOfstepsWithoutImprovement = 0;

    bool done = false;
    int nOfSteps = 0;
    int nOfAllCheckedResults = 0;

    while (!done)
    {
        // std::cout << "Current result: " << currentPermutationValue << std::endl;
        bool isNewCurrentResult = false;
        for (int i = 0; i < nOfCities; i++)
        {
            for (int j = 1 + i; j < nOfCities; j++)
            {
                int neighbourValue = ComputePossibleValue(currentPermutation, currentPermutationValue, distanceMatrix, Pair<int>(i, j), nOfCities);
                nOfAllCheckedResults++;
                // std::cout << "curr: " << currentPermutationValue << "\tneigh: " << neighbourValue << std::endl;
                // std::cout << "i: " << i << "\tj: " << j << std::endl;
                // std::cout << "currentPermutationValue: " << currentPermutationValue << std::endl;
                // std::cout << "neighbourValue: " << neighbourValue << std::endl;
                if (neighbourValue < currentPermutationValue)
                {
                    std::swap(currentPermutation[i], currentPermutation[j]);
                    currentPermutationValue = neighbourValue;
                    nOfstepsWithoutImprovement += 1;

                    // if ((resultStruct.iterationNumber != -1) && (currentPermutationValue < bestPermutationValue))
                    // {
                    //     std::copy(currentPermutation, currentPermutation + nOfCities, bestPermutation); // TODO
                    // }
                    // valueChange = bestPermutationValue - currentPermutationValue;
                    if (currentPermutationValue < bestPermutationValue)
                    {
                        // std::cout << "bestPermutationValue: " << bestPermutationValue << std::endl;
                        // std::cout << "currentPermutationValue: " << currentPermutationValue << std::endl;
                        // std::cout << "===================\n";
                        bestPermutationValue = currentPermutationValue;
                        nOfstepsWithoutImprovement = 0;
                    }

                    isNewCurrentResult = true;
                    nOfSteps++;
                    stepsWthoutCChange++;
                    break;
                }
                else if (neighbourValue > currentPermutationValue)
                {
                    nOfstepsWithoutImprovement++;
                    stepsWthoutCChange++;
                    nOfSteps++;
                    float randomNumber = (float)std::rand() / RAND_MAX;
                    float prob = std::exp((-neighbourValue - currentPermutationValue) / C);
                    if (prob > randomNumber)
                    {
                        // std::cout << "GORSZY SWAP\n";
                        std::swap(currentPermutation[i], currentPermutation[j]);
                        currentPermutationValue = neighbourValue;
                        isNewCurrentResult = true;
                        break;
                    }
                }
                // else if (i == nOfCities - 2 && j == nOfCities - 1)
                //     done = true;

                isNewCurrentResult = false;
            }
            if (isNewCurrentResult)
                break;
        }

        if (nOfstepsWithoutImprovement > maxNOfstepsWithouImprovement)
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

int Tabu(Matrix distanceMatrix, int nOfCities, ResultStruct &resultStruct)
{

    int max_neighbour = (nOfCities * (nOfCities - 1)) / 2;
    PairV<int> *neighbour_choices = new PairV<int>[max_neighbour];
    int n_c_it = 0;
    int no_change_diff = 0;

    int analyzed_neighbours = nOfCities / 10; ///1 sąsiad  2-4

    int sizeOfTabu = nOfCities / 4; // 4 tabu 2-4
    int tabuIt = 0;
    int possible_value = 0;
    PairV<int> *tabu = new PairV<int>[sizeOfTabu];

    int max_val = 0;
    int p = 10;
    int l = max_neighbour;
    int max_repetition = l * p; // L = (nOfCities^2)/2

    // std::cout<<"analyzed_neighbours: "<<analyzed_neighbours<<std::endl;
    // std::cout<<"sizeOfTabu: "<<sizeOfTabu<<std::endl;

    // initial random permutation
    int *currentPermutation = RandomPermutation(nOfCities);
    int *currentDistancesArray = GetArrayOfDistances(currentPermutation, nOfCities, distanceMatrix);
    int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);

    int *bestPermutation = new int[nOfCities];
    std::copy(currentPermutation, currentPermutation + nOfCities, bestPermutation);
    if (resultStruct.iterationNumber != -1)
        resultStruct.initialResult = currentPermutationValue;

    delete[] currentDistancesArray;
    int bestPermutationValue = currentPermutationValue;
    bool done = false;
    int nOfSteps = 0;
    int nOfAllCheckedResults = 0;
    int it = 0;

    for (int i = 0; i < sizeOfTabu; i++)
    {
        tabu[i] = PairV(0, 0, -10000);
    }

    while (no_change_diff < max_repetition)
    {
        done = false;
        n_c_it = 0;
        for (int i = 0; i < nOfCities; i++)
        {
            for (int j = 1 + i; j < nOfCities; j++)
            {
                possible_value = ComputePossibleValue(currentPermutation, 0, distanceMatrix, Pair<int>(i, j), nOfCities);
                neighbour_choices[n_c_it] = PairV(i, j, possible_value);
                n_c_it++;
                nOfAllCheckedResults++;
            }
        }
        no_change_diff += max_neighbour - analyzed_neighbours;
        n_c_it = 0;
        std::sort(neighbour_choices, neighbour_choices + max_neighbour, comparePairV);
        // printTabu(neighbour_choices,analyzed_neighbours,0);
        // exit(0);
        max_val = neighbour_choices[analyzed_neighbours - 1].value;

        while (!done && n_c_it < analyzed_neighbours)
        {

            for (int i = n_c_it; i < analyzed_neighbours; i++)
            {
                if ((notOnTabuList(tabu, sizeOfTabu, neighbour_choices[i])) || (bestPermutationValue > (currentPermutationValue + neighbour_choices[i].value)))
                {

                    if (neighbour_choices[i].value <= max_val)
                    {
                        std::swap(currentPermutation[neighbour_choices[i].first], currentPermutation[neighbour_choices[i].second]);
                        currentPermutationValue = currentPermutationValue + neighbour_choices[i].value;

                        pushTabu(tabu, sizeOfTabu, tabuIt, PairV(neighbour_choices[i].first, neighbour_choices[i].second, neighbour_choices[i].value));
                        tabuIt--;

                        if (currentPermutationValue < bestPermutationValue)
                        {
                            bestPermutationValue = currentPermutationValue;
                            if ((resultStruct.iterationNumber != -1))
                            {
                                std::copy(currentPermutation, currentPermutation + nOfCities, bestPermutation);
                            }
                            no_change_diff = 0;
                        }
                        else
                        {
                            no_change_diff++;
                        }
                        if (n_c_it != i)
                        {
                            std::swap(neighbour_choices[i], neighbour_choices[n_c_it]);
                        }
                    }
                    else
                    {
                        no_change_diff++;
                        done = true;
                    }
                    n_c_it++;
                    break;
                }
                else
                {
                    n_c_it++;
                    no_change_diff++;
                }
            }

            if (!done)
            {
                for (int i = n_c_it; i < analyzed_neighbours; i++)
                {
                    neighbour_choices[i].value = ComputePossibleValue(currentPermutation, 0, distanceMatrix,
                                                                      Pair<int>(neighbour_choices[i].first, neighbour_choices[i].second), nOfCities);
                    nOfAllCheckedResults++;
                }
                std::sort(neighbour_choices + n_c_it, neighbour_choices + analyzed_neighbours, comparePairV);
            }
        }
    }
    // std::cout<<"BEST PERM: "<<bestPermutationValue<<std::endl;
    if (resultStruct.iterationNumber != -1)
    {
        resultStruct.finalResult = bestPermutationValue;
        // resultStruct.numberOfSteps = nOfSteps;
        resultStruct.numberOfCheckedResults = nOfAllCheckedResults;
        resultStruct.resultPermutation = std::vector<int>(bestPermutation, bestPermutation + nOfCities);
    }
    return 0;
}

#endif