#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
// #include <iterator>
#include "matrix.h"
#include "util.h"
#include "algorithms.h"

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
    string filepath;
    string config_file;
    try
    {
        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        // tutdaj jest przeciazony operator () i jak zrobie auto format to tak brzydko wyglada niestety
        generic.add_options()
            ("help", "Produce help message.")
            ("config,c", po::value<string>(&config_file)->default_value("config.cfg"), "Name of a file of a configuration.");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("atsp-file", po::value<string>(&filepath), "Path to file containing data for ATSP problem.");


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
            return 0;
        }

        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "Can not open config file: " << config_file << "\n";
            return 0;
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
        return 1;
    }

    srand(SEED);
    std::cout << "PLIK: " << filepath << std::endl;

    Pair<std::string> optimalResults[OPTIMAL_RESULTS_NUMBER];
    LoadOptimalResults(OPTIMAL_RESULTS_PATH, optimalResults);

    int nOfCities;
    Matrix distanceMatrix = LoadData(filepath, nOfCities);
    // std::cout << distanceMatrix;

    // MeasureTimeOfFunctionInMilliSeconds(1, "Greedy",  Greedy, distanceMatrix, nOfCities);
    // MeasureTimeOfFunctionInMilliSeconds(1, "SimpleHeur", SimpleHeur, distanceMatrix, nOfCities);

    MeasureTimeOfFunctionInMilliSeconds(1, "Greedy", Greedy, distanceMatrix, nOfCities);
    MeasureTimeOfFunctionInMilliSeconds(1, "Steepest", Steepest, distanceMatrix, nOfCities);

    // result = SimpleHeur(distanceMatrix, nOfCities);
    // std::cout << "SimpleHeur: " << result << std::endl;

    int result = Greedy(distanceMatrix, nOfCities);
    std::cout << "Greedy: " << result << std::endl;

    result = Steepest(distanceMatrix, nOfCities);
    std::cout << "Steepest: " << result << std::endl;

    // RunFuncNTimes(3, Greedy, distanceMatrix, nOfCities);
}
