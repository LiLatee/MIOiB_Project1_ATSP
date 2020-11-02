#include <iostream>
#include "matrix.h"
#include "util.h"
#include "algorithms.h"

const int OPTIMAL_RESULTS_NUMBER = 27;
const std::string OPTIMAL_RESULTS_PATH = "./data/unpacked_ready/optimal_results.csv";

template <typename T, typename... Args>
void RunFuncNTimes(const int n, T func, Args &&... args)
{
    for (int i = 0; i < n; i++)
    {
        func(std::forward<Args>(args)...);
    }
}

// const int SEED = TimeSinceEpochMillisec(); 
const int SEED = 44; 

int main(int argc, char **argv)
{
    srand(SEED);
    std::string filepath = argv[1];
    std::cout << "PLIK: " << filepath << std::endl;

    Pair<std::string> optimalResults[OPTIMAL_RESULTS_NUMBER];
    LoadOptimalResults(OPTIMAL_RESULTS_PATH, optimalResults);

    int nOfCities;
    Matrix distanceMatrix = LoadData(filepath, nOfCities);
    // std::cout << distanceMatrix;

    MeasureTimeOfFunctionInMilliSeconds(1, "Greedy",  Greedy, distanceMatrix, nOfCities);
    // MeasureTimeOfFunctionInMilliSeconds(1, "SimpleHeur", SimpleHeur, distanceMatrix, nOfCities);
    // MeasureTimeOfFunctionInMilliSeconds(10, "Steepest", Steepest, distanceMatrix, nOfCities);

    // int result = Greedy(distanceMatrix, nOfCities);
    // std::cout << "Greedy: " << result << std::endl;

    // result = SimpleHeur(distanceMatrix, nOfCities);
    // std::cout << "SimpleHeur: " << result << std::endl;

    // result = Steepest(distanceMatrix, nOfCities);
    // std::cout << "Steepest: " << result << std::endl;

    // std::vector<Pair<std::string>> result = LoadOptimalResults("./data/unpacked_ready/optimal_results.csv");
    // std::cout << result[0].first << std::endl;

    // RunFuncNTimes(3, Greedy, distanceMatrix, nOfCities);
}
