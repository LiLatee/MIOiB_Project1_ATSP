#ifndef UTIL_CPP_
#define UTIL_CPP_

#include <iostream>
#include <numeric>
#include <regex>
#include <map>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <filesystem>
namespace fs = std::filesystem;

uint64_t TimeSinceEpochMillisec()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

struct ResultStruct
{
    std::string setName = "default set name";
    std::string algName = "default alg name";
    int nOfCities = -1;
    int iterationNumber = -1;
    int result = -1;
    int optimalResult = -1;
    int numberOfSteps = -1;
    int numberOfCheckedResults = -1;
    double timeOfRunningInMs = -1.0;
    std::vector<int> resultPermutation{1, 2, 3, 4, 5};
};

std::ostream &operator<<(std::ostream &out, const ResultStruct &resultStruct)
{
    out << "setName: " << resultStruct.setName << std::endl;
    out << "algName: " << resultStruct.algName << std::endl;
    out << "nOfCities: " << resultStruct.nOfCities << std::endl;
    out << "iterationNumber: " << resultStruct.iterationNumber << std::endl;
    out << "result: " << resultStruct.result << std::endl;
    out << "numberOfSteps: " << resultStruct.numberOfSteps << std::endl;
    out << "numberOfCheckedResults: " << resultStruct.numberOfCheckedResults << std::endl;
    out << "timeOfRunningInMs: " << resultStruct.timeOfRunningInMs << std::endl;
    out << "resultPermutation: ";
    for (auto i = resultStruct.resultPermutation.begin(); i != resultStruct.resultPermutation.end(); ++i)
        out << *i << ',';

    out << std::endl;

    return out;
}

template <typename T>
struct Pair
{
    T first;
    T second;

    Pair()
    {
        this->first = T();
        this->second = T();
    }
    Pair(T first, T second)
    {
        this->first = first;
        this->second = second;
    }

    T &operator[](int index)
    {
        if (index == 0)
            return this->first;
        else if (index == 1)
            return this->second;
        else
        {
            std::cout << "Object pair has only 0 and 1 indexes.";
            exit(0);
        }
    }
};

// TODO gdzieś to jest używane?
// template <class T>
// std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
// {
//     copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
//     return os;
// }

void LoadParameters(int &argc, char **argv, std::string &setsPath, std::string &resultsFilePath, int &nOfIterations, std::string &optimalResultsPath)
{
    std::string config_file;
    try
    {
        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        // tutdaj jest przeciazony operator () i jak zrobie auto format to tak brzydko wyglada niestety
        generic.add_options()("help", "Produce help message.")("config,c", po::value<std::string>(&config_file)->default_value("config.cfg"), "Name of a file of a configuration.");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("sets-path,sp", po::value<std::string>(&setsPath), "Path to directory containing data for ATSP problem.")
            ("results-file,rf", po::value<std::string>(&resultsFilePath), "Path to directory for results.")
            ("n-of-iterations,nof", po::value<int>(&nOfIterations), "Number of iterations of one algorithm on each set.")
            ("opt-results-file,orp", po::value<std::string>(&optimalResultsPath), "Path to file with optimal results.");
    
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
            std::cout << visible << "\n";
            exit(0);
        }

        std::ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            std::cout << "Can not open config file: " << config_file << "\n";
            exit(0);
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
        exit(0);
    }
}

void SaveResultsToCSV(std::vector<ResultStruct> results, fs::path resultsFilePath)
{
    if (!fs::exists(resultsFilePath.parent_path()))
    {
        fs::create_directory(resultsFilePath.parent_path());
    }
    std::ofstream file;
    fs::file_status status = fs::file_status{};
    bool isFileExist = fs::exists(resultsFilePath);
    
    if (isFileExist)
        file.open(resultsFilePath, std::ios::app);
    else
        file.open(resultsFilePath);

    if (file.is_open())
    {
        if (!isFileExist)
        {
            file << "setName,";
            file << "algName,";
            file << "nOfCities,";
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
            file << it->nOfCities << ",";
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


int SumOfarray(int arr[], int size)
{
    return std::accumulate(arr, arr + size, 0, std::plus<int>());
}

template <typename T>
void PrintVector(T *const arr, int const size)
{

    for (T i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;
}


template <typename T, typename... Args>
double MeasureTimeOfFunctionInMilliSeconds(int testDurationInSecs, std::string funcName, T func, Args &&... args)
{
    int counter = 0;
    uint64_t start = TimeSinceEpochMillisec();
    do
    {
        func(std::forward<Args>(args)...); // funkcja do zmierzenia czasu
        counter++;
    } while (TimeSinceEpochMillisec() - start < testDurationInSecs * 1000);

    int timeForAllRuns = (TimeSinceEpochMillisec() - start);
    double result = double(timeForAllRuns) / double(counter);

    std::cout << "Czas dla: " << funcName << "\t" << result << " ms" << std::endl;
    return result;
}

/**
 * Wykonuje funkcje 'n' razy.
 **/
template <typename T, typename... Args>
double MeasureTimeOfFunctionInMilliSecondsV2(int nRuns, std::string funcName, T func, Args &&... args)
{
    uint64_t start = TimeSinceEpochMillisec();
    for (size_t i = 0; i < nRuns; i++)
        func(std::forward<Args>(args)...); // funkcja do zmierzenia czasu

    int timeForAllRuns = (TimeSinceEpochMillisec() - start);
    double result = double(timeForAllRuns) / double(nRuns);

    std::cout << "Czas dla: " << funcName << "\t" << result << " ms" << std::endl;
    return result;
}

Matrix LoadData(std::string const filepath, int &nOfCitiesOut)
{
    std::ifstream file(filepath);
    if (file.is_open())
    {
        std::string line;
        // // omijamy pierwsze 6 linii
        for (int i = 0; i < 7; i++)
        {
            std::getline(file, line);
            if (std::regex_match(line, std::regex("^DIMENSION:[ ]*[0-9]+")))
            {
                std::smatch m;
                std::regex_search(line, m, std::regex("[0-9]+"));
                nOfCitiesOut = std::stoi(m[0]);
            }
        }
        std::cout << "LICZBA MIAST: " << nOfCitiesOut << std::endl;
        Matrix matrix = Matrix(nOfCitiesOut, nOfCitiesOut);
        std::string word;
        for (int i = 0; i < nOfCitiesOut; i++)
            for (int j = 0; j < nOfCitiesOut; j++)
            {
                file >> word;
                matrix.SetValue(i, j, std::stoi(word));
            }
        file.close();
        return matrix;
    }
    else
    {
        std::cout << "Nie udało się wczytać pliku: " << filepath << std::endl;
        exit(0);
    }
}


void LoadOptimalResults(std::string const filepath, std::map<std::string, int> &optimalresultsOut)
{
    std::ifstream file(filepath);

    if (file.is_open())
    {
        std::string line;
        int cnt = 0;
        while (std::getline(file, line))
        {
            std::string word;
            std::istringstream stream;
            stream.str(line);

            std::string setName;
            stream >> setName;
            std::string setOptimalResult;
            stream >> setOptimalResult;

            optimalresultsOut[setName] = std::stoi(setOptimalResult);
        }
    }
    else
    {
        std::cout << "Can not read file: " << filepath << std::endl;
        exit(0);
    }
}


#endif