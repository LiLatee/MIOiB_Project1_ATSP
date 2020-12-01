#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "matrix.h"
#include "util.cpp"
#include "algorithms.h"
#include <map>

using namespace std;

void RunTests(const string setsPath, const string resultsFilePath, const int nOfIterations, const string optimalResultsPath)
{
    std::map<string, int> optimalResults;
    LoadOptimalResults(optimalResultsPath, optimalResults);

    for (const auto &entry : fs::directory_iterator(setsPath))
    {
        std::string setName = entry.path().stem();
        std::cout << "Set: \t " << setName << std::endl;
        int nOfCities;
        std::string path ="/mnt/d/Dokumenty/Uczelnia/MIOiB/Project1_ATSP/data/unpacked_ready/chosen_sets/p43.atsp";
        Matrix distanceMatrix = LoadData(entry.path(), nOfCities);

        ResultStruct result;
        // mierzymy czas, aby Random i RandomWalker miały mniej więcej tyle samu czasu.
        double greedyTime = MeasureTimeOfFunctionInMilliSeconds(1, "Greedy", Greedy, distanceMatrix, nOfCities, result);
        double steepestTime = MeasureTimeOfFunctionInMilliSeconds(1, "Steepest", Steepest, distanceMatrix, nOfCities, result);
        double SWTime = MeasureTimeOfFunctionInMilliSeconds(1, "SW", SW, distanceMatrix, nOfCities, result);
        double TabuTime = MeasureTimeOfFunctionInMilliSeconds(1, "Tabu", Tabu, distanceMatrix, nOfCities, result);
        // double SW2Time = MeasureTimeOfFunctionInMilliSeconds(1, "SW2", SW2, distanceMatrix, nOfCities, result);

        double simpleHeurTime = MeasureTimeOfFunctionInMilliSeconds(1, "SimpleHeur", SimpleHeur, distanceMatrix, nOfCities, result);
        // double timeInMillisecForRandoms = (greedyTime + steepestTime) / (2);
        // std::cout << "timeInMillisecForRandoms: " << timeInMillisecForRandoms << std::endl;

        vector<ResultStruct> steepestResults;
        for (size_t i = 0; i < nOfIterations; i++)
        {
            ResultStruct result;
            result.setName = setName;
            result.algName = "Steepest";
            result.nOfCities = nOfCities;
            result.iterationNumber = i;
            result.optimalResult = optimalResults[setName];
            result.timeOfRunningInMs = steepestTime;
            Greedy(distanceMatrix, nOfCities, result);
            steepestResults.push_back(result);
        }
        SaveResultsToCSV(steepestResults, resultsFilePath);

        vector<ResultStruct> greedyResults;
        for (size_t i = 0; i < nOfIterations; i++)
        {
            ResultStruct result;
            result.setName = setName;
            result.algName = "Greedy";
            result.nOfCities = nOfCities;
            result.iterationNumber = i;
            result.optimalResult = optimalResults[setName];
            result.timeOfRunningInMs = greedyTime;
            Greedy(distanceMatrix, nOfCities, result);
            greedyResults.push_back(result);
        }
        SaveResultsToCSV(greedyResults, resultsFilePath);

        vector<ResultStruct> simpleHeurResults;
        for (size_t i = 0; i < nOfIterations; i++)
        {
            ResultStruct result;
            result.setName = setName;
            result.algName = "SimpleHeur";
            result.nOfCities = nOfCities;
            result.iterationNumber = i;
            result.optimalResult = optimalResults[setName];
            result.timeOfRunningInMs = simpleHeurTime;
            SimpleHeur(distanceMatrix, nOfCities, result);
            simpleHeurResults.push_back(result);
        }
        SaveResultsToCSV(simpleHeurResults, resultsFilePath);

        // vector<ResultStruct> randomResults;
        // for (size_t i = 0; i < nOfIterations; i++)
        // {
        //     ResultStruct result;
        //     result.setName = setName;
        //     result.algName = "Random";
        //     result.nOfCities = nOfCities;
        //     result.iterationNumber = i;
        //     result.optimalResult = optimalResults[setName];
        //     result.timeOfRunningInMs = timeInMillisecForRandoms;
        //     Random(distanceMatrix, nOfCities, timeInMillisecForRandoms, result);
        //     randomResults.push_back(result);
        // }
        // SaveResultsToCSV(randomResults, resultsFilePath);

        // vector<ResultStruct> randomWalkResults;
        // for (size_t i = 0; i < nOfIterations; i++)
        // {
        //     ResultStruct result;
        //     result.setName = setName;
        //     result.algName = "RandomWalk";
        //     result.nOfCities = nOfCities;
        //     result.iterationNumber = i;
        //     result.optimalResult = optimalResults[setName];
        //     result.timeOfRunningInMs = timeInMillisecForRandoms;
        //     RandomWalk(distanceMatrix, nOfCities, timeInMillisecForRandoms, result);
        //     randomWalkResults.push_back(result);
        // }
        // SaveResultsToCSV(randomWalkResults, resultsFilePath);

        vector<ResultStruct> SWResults;
        for (size_t i = 0; i < nOfIterations; i++)
        {
            ResultStruct result;
            result.setName = setName;
            result.algName = "SW";
            result.nOfCities = nOfCities;
            result.iterationNumber = i;
            result.optimalResult = optimalResults[setName];
            result.timeOfRunningInMs = SWTime;
            SW(distanceMatrix, nOfCities, result);
            SWResults.push_back(result);
        }
        SaveResultsToCSV(SWResults, resultsFilePath);

        // vector<ResultStruct> SW2Results;
        // for (size_t i = 0; i < nOfIterations; i++)
        // {
        //     ResultStruct result;
        //     result.setName = setName;
        //     result.algName = "SW2";
        //     result.nOfCities = nOfCities;
        //     result.iterationNumber = i;
        //     result.optimalResult = optimalResults[setName];
        //     result.timeOfRunningInMs = SW2Time;
        //     SW2(distanceMatrix, nOfCities, result);
        //     SW2Results.push_back(result);
        // }
        // SaveResultsToCSV(SW2Results, resultsFilePath);
        
        vector<ResultStruct> TabuResults;
        for (size_t i = 0; i < nOfIterations; i++)
        {
            ResultStruct result;
            result.setName = setName;
            result.algName = "Tabu";
            result.nOfCities = nOfCities;
            result.iterationNumber = i;
            result.optimalResult = optimalResults[setName];
            result.timeOfRunningInMs = TabuTime;
            SW(distanceMatrix, nOfCities, result);
            TabuResults.push_back(result);
        }
        SaveResultsToCSV(TabuResults, resultsFilePath);

        // TODO to samo dla SimpleHeur, R i RW dodać
    }
}

const int SEED = TimeSinceEpochMillisec();
// const int SEED = 44;
int main(int argc, char **argv)
{
    srand(SEED);
    string setsPath, resultsFilePath, optimalResultsPath;
    int nOfIterations;
    LoadParameters(argc, argv, setsPath, resultsFilePath, nOfIterations, optimalResultsPath);

    std::cout << "Sets path: " << setsPath << std::endl;
    std::cout << "Results path: " << resultsFilePath << std::endl;
    RunTests(setsPath, resultsFilePath, nOfIterations, optimalResultsPath);

    //// testowanie ComputePossibleValue
    // int parentPermutation[] = {0,1,2,3,4};
    // int nOfCities = 5;
    // Matrix distanceMatrix = Matrix(5, 5);
    // for (int i = 0; i < 5; i++)
    //     for (int j = 0; j < 5; j++)
    //         distanceMatrix.SetValue(i, j, i * 5 + j);

    // std::cout << distanceMatrix;

    // int *currentDistancesArray = GetArrayOfDistances(parentPermutation, nOfCities, distanceMatrix);
    // int currentPermutationValue = SumOfarray(currentDistancesArray, nOfCities);
    // int parentValue = SumOfarray(currentDistancesArray, nOfCities);
    // std::cout << "====================================PRZED====================================\n";
    // PrintVector(parentPermutation, nOfCities);
    // PrintVector(currentDistancesArray, nOfCities);
    // std::cout << "parentValue: " << parentValue << std::endl;

    // int p1 = std::stoi(argv[1]);
    // int p2 = std::stoi(argv[2]);
    // Pair<int> swappedIndexes = Pair(p1, p2);
    // int newValue = ComputePossibleValue(parentPermutation, parentValue, distanceMatrix, swappedIndexes, nOfCities);
    // int newPermutation[nOfCities];
    // std::copy(parentPermutation, parentPermutation+nOfCities, newPermutation);
    // std::swap(newPermutation[p1], newPermutation[p2]);
    // int *newDistancesArray = GetArrayOfDistances(newPermutation, nOfCities, distanceMatrix);
    // std::cout << "====================================PO====================================\n";
    // PrintVector(newPermutation, nOfCities);
    // PrintVector(newDistancesArray, nOfCities);
    // std::cout << "newValue: " << newValue << std::endl;
}
