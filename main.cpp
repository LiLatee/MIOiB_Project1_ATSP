#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "matrix.h"
#include "util.h"
#include "algorithms.h"
#include "random.cpp"
#include <map>

#include <filesystem>
namespace fs = std::filesystem;

#include <boost/program_options.hpp>
namespace po = boost::program_options;
const int OPTIMAL_RESULTS_NUMBER = 27;
const std::string OPTIMAL_RESULTS_PATH = "./data/unpacked_ready/optimal_results.csv";

using namespace std;

template <class T>
ostream &operator<<(ostream &os, const vector<T> &v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
    return os;
}

void LoadParameters(int &argc, char **argv, string &setsPath, string &resultsFilePath)
{
    string config_file;
    try
    {
        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        // tutdaj jest przeciazony operator () i jak zrobie auto format to tak brzydko wyglada niestety
        generic.add_options()("help", "Produce help message.")("config,c", po::value<string>(&config_file)->default_value("config.cfg"), "Name of a file of a configuration.");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()("sets-path", po::value<string>(&setsPath), "Path to directory containing data for ATSP problem.")("results-file", po::value<string>(&resultsFilePath), "Path to directory for results.");

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config);

        po::options_description config_file_options;
        config_file_options.add(config);

        po::options_description visible("Allowed options");
        visible.add(generic).add(config);

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
        notify(vm);

        if (vm.count("help"))
        {
            cout << visible << "\n";
            exit(0);
        }

        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "Can not open config file: " << config_file << "\n";
            exit(0);
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }
    }
    catch (exception &e)
    {
        cout << e.what() << "\n";
        exit(0);
    }
}

template <typename T, typename... Args>
void RunFuncNTimes(const int n, T func, Args &&... args)
{
    for (int i = 0; i < n; i++)
    {
        func(std::forward<Args>(args)...);
    }
}

void SaveResultsToCSV(std::vector<ResultStruct> results, fs::path resultsFilePath)
{
    if (!fs::exists(resultsFilePath.parent_path()))
    {
        fs::create_directory(resultsFilePath.parent_path());
    }
    ofstream file;
    fs::file_status status = fs::file_status{};
    bool isFileExist = fs::exists(resultsFilePath);
    
    if (isFileExist)
        file.open(resultsFilePath, ios::app);
    else
        file.open(resultsFilePath);

    if (file.is_open())
    {
        if (!isFileExist)
        {
            file << "setName,";
            file << "algName,";
            file << "iterationNumber,";
            file << "result,";
            file << "optimalResult,";
            file << "numberOfSteps,";
            file << "numberOfCheckedResults,";
            file << "timeOfRunningInMs,";
            file << "resultPermutation";
            file << std::endl;
        }

        for (auto it = results.begin(); it != results.end(); it++)
        {
            file << it->setName << ",";
            file << it->algName << ",";
            file << it->iterationNumber << ",";
            file << it->result << ",";
            file << it->optimalResult << ",";
            file << it->numberOfSteps << ",";
            file << it->numberOfCheckedResults << ",";
            file << it->timeOfRunningInMs << ",";
            for (auto it2 = it->resultPermutation.begin(); it2 != it->resultPermutation.end(); it2++)
                file << *it2 << "|";
            file << std::endl;
        }
        file.close();
    }
}

void RunTests(const string setsPath, string resultsFilePath)
{
    const int nOfIterations = 10;
    // Pair<std::string> optimalResults[OPTIMAL_RESULTS_NUMBER];
    std::map<string, int> optimalResults;
    LoadOptimalResultsV2(OPTIMAL_RESULTS_PATH, optimalResults);

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
    string setsPath, resultsFilePath;
    LoadParameters(argc, argv, setsPath, resultsFilePath);
    srand(SEED);
    std::cout << "Sets path: " << setsPath << std::endl;
    std::cout << "Results path: " << resultsFilePath << std::endl;
    RunTests(setsPath, resultsFilePath);
    exit(0);

    // Pair<std::string> optimalResults[OPTIMAL_RESULTS_NUMBER];
    // LoadOptimalResults(OPTIMAL_RESULTS_PATH, optimalResults);

    // int nOfCities;
    // Matrix distanceMatrix = LoadData(filepath, nOfCities);
    // // std::cout << distanceMatrix;

    // // MeasureTimeOfFunctionInMilliSeconds(1, "Greedy",  Greedy, distanceMatrix, nOfCities);
    // // MeasureTimeOfFunctionInMilliSeconds(1, "SimpleHeur", SimpleHeur, distanceMatrix, nOfCities);

    // MeasureTimeOfFunctionInMilliSeconds(1, "Greedy", Greedy, distanceMatrix, nOfCities);
    // MeasureTimeOfFunctionInMilliSeconds(1, "Steepest", Steepest, distanceMatrix, nOfCities);

    // // result = SimpleHeur(distanceMatrix, nOfCities);
    // // std::cout << "SimpleHeur: " << result << std::endl;

    // // int result = Greedy(distanceMatrix, nOfCities);
    // // std::cout << "Greedy: " << result << std::endl;

    // // result = Steepest(distanceMatrix, nOfCities);
    // // std::cout << "Steepest: " << result << std::endl;

    // RunFuncNTimes(3, Greedy, distanceMatrix, nOfCities);
    // // RunFuncNTimes(3, Greedy, distanceMatrix, nOfCities);
}
