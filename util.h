#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <numeric>
#include <regex>
#include "matrix.h"
#include <map>

#ifndef UTIL_H_
#define UTIL_H_

uint64_t TimeSinceEpochMillisec()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

struct ResultStruct
{
    std::string setName = "default set name";
    std::string algName = "default alg name";
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

int *RandomPermutation(int const n)
{
    int *arr = new int[n];

    std::iota(arr, arr + n, 0);

    // TODO ODKOMENTOWAĆ

    // losowanie permutacji
    int temp;
    for (int i = 0; i < n; i++)
    {
        int randomIndex = rand() % (n - i); // (n-i), bo po usunieciu elementu przesuwamy index w lewo
        std::swap(arr[n - 1 - i], arr[randomIndex]);
    }
    return arr;
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

void LoadOptimalResults(std::string const filepath, Pair<std::string> *optimalresultsOut)
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

            optimalresultsOut[cnt++] = Pair<std::string>(setName, setOptimalResult);
            // std::cout << pair.first << "\t" << pair.second << std::endl;
        }
    }
    else
    {
        std::cout << "Nie udało się wczytać pliku: " << filepath << std::endl;
        exit(0);
    }
}

void LoadOptimalResultsV2(std::string const filepath, std::map<std::string, int> &optimalresultsOut)
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
            // std::cout << pair.first << "\t" << pair.second << std::endl;
        }
    }
    else
    {
        std::cout << "Nie udało się wczytać pliku: " << filepath << std::endl;
        exit(0);
    }
}

/**
 * Zwraca tablice wartości odległości między miastami w permutacji `arr`.
 */
int *GetArrayOfDistances(int arr[], int size, Matrix &distanceMatrix)
{
    int *result = new int[size];
    for (size_t i = 1; i < size; i++)
    {
        int distance = distanceMatrix.GetValue(arr[i - 1], arr[i]);
        result[i - 1] = distance;
    }

    // dodanie odleglosci laczacej ostatni z pierwszym
    int distance = distanceMatrix.GetValue(arr[size - 1], arr[0]);
    result[size - 1] = distance;
    return result;
}

/**
 * Na podstawie permutacji rodzica 'parentPermutation', wartości rodzica 'parentValue', 
 * macierzy odległości 'distanceMatrix' wylicza nową wartość dla permutacji, 
 * która ma zmienione indeksy względem rodzica zapisane w 'swappedIndex'.
 */
int ComputePossibleValue(const int parentPermutation[], const int parentValue, const Matrix &distanceMatrix, const Pair<int> swappedIndexes, const int nOfCities)
{
    int possibleValue = parentValue;

    int oldValue;
    int newValue;
    // std::cout << "swappedIndexes.first: " << swappedIndexes.first << "\tswappedIndexes.second: " << swappedIndexes.second << std::endl;
    if (swappedIndexes.first == 0 && swappedIndexes.second == nOfCities - 1)
    {
        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.first]); // 1
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.second]); // 1
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.first + 1]); // 2
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.first + 1]); // 2
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second - 1], parentPermutation[swappedIndexes.second]); // 3
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second - 1], parentPermutation[swappedIndexes.first]); // 3
        possibleValue += newValue;
        return possibleValue;
    }
    else if (swappedIndexes.first == 0 && swappedIndexes.second == 1)
    {
        oldValue = distanceMatrix.GetValue(parentPermutation[nOfCities - 1], parentPermutation[swappedIndexes.first]); // 1
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[nOfCities - 1], parentPermutation[swappedIndexes.second]); // 1
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.second]); // 2
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.first]); // 2
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.second + 1]); // 3
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.second + 1]); // 3
        possibleValue += newValue;
        return possibleValue;
    }
    else if (swappedIndexes.first == nOfCities - 2 && swappedIndexes.second == nOfCities - 1)
    {
        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first - 1], parentPermutation[swappedIndexes.first]); // 1
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first - 1], parentPermutation[swappedIndexes.second]); // 1
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.second]); // 2
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.first]); // 2
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[0]); // 3
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[0]); // 3
        possibleValue += newValue;
        return possibleValue;
    }
    else if (abs(swappedIndexes.first - swappedIndexes.second) == 1)
    {
        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first - 1], parentPermutation[swappedIndexes.first]); // 1
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first - 1], parentPermutation[swappedIndexes.second]); // 1
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.second]); // 2
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.first]); // 2
        possibleValue += newValue;

        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[swappedIndexes.second + 1]); // 3
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[swappedIndexes.second + 1]); // 3
        possibleValue += newValue;
        return possibleValue;
    }
    else
    {
        int firstChangedValueIndex = swappedIndexes.first - 1;
        if (firstChangedValueIndex == -1)
        {
            firstChangedValueIndex = nOfCities - 1;
        }
        oldValue = distanceMatrix.GetValue(parentPermutation[firstChangedValueIndex], parentPermutation[swappedIndexes.first]); //-a
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[firstChangedValueIndex], parentPermutation[swappedIndexes.second]); //+a
        possibleValue += newValue;

        int secondChangedValueIndex = swappedIndexes.first + 1;
        if (secondChangedValueIndex == nOfCities)
            secondChangedValueIndex = 0;
        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[secondChangedValueIndex]); //-b
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[secondChangedValueIndex]); //+b
        possibleValue += newValue;

        firstChangedValueIndex = swappedIndexes.second - 1;
        if (firstChangedValueIndex == -1)
            firstChangedValueIndex = nOfCities - 1;
        oldValue = distanceMatrix.GetValue(parentPermutation[firstChangedValueIndex], parentPermutation[swappedIndexes.second]); //-c
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[firstChangedValueIndex], parentPermutation[swappedIndexes.first]); //+c
        possibleValue += newValue;

        // indeks elementu po zamienionym elemencie
        secondChangedValueIndex = swappedIndexes.second + 1;
        if (secondChangedValueIndex == nOfCities)
            secondChangedValueIndex = 0;
        oldValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.second], parentPermutation[secondChangedValueIndex]); //-d
        possibleValue -= oldValue;
        newValue = distanceMatrix.GetValue(parentPermutation[swappedIndexes.first], parentPermutation[secondChangedValueIndex]); //+d
        possibleValue += newValue;

        return possibleValue;
    }
}

#endif