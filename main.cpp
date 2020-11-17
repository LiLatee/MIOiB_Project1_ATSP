#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "matrix.h"
#include "util.cpp"
#include "algorithms.h"
#include "random.cpp"
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
        Matrix distanceMatrix = LoadData(entry.path(), nOfCities);

        ResultStruct result;
        // mierzymy czas, aby Random i RandomWalker miały mniej więcej tyle samu czasu.
        double greedyTime = MeasureTimeOfFunctionInMilliSeconds(1, "Greedy", Greedy, distanceMatrix, nOfCities, result);
        double steepestTime = MeasureTimeOfFunctionInMilliSeconds(1, "Steepest", Steepest, distanceMatrix, nOfCities, result);
        double simpleHeurTime = MeasureTimeOfFunctionInMilliSeconds(1, "SimpleHeur", SimpleHeur, distanceMatrix, nOfCities);
        double timeInSecForRandoms = (greedyTime + steepestTime) / 2 * 1000;

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
}
