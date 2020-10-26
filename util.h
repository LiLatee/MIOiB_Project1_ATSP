#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <numeric>
// #include <filesystem>
#include "matrix.h"
#ifndef UTIL_H_
#define UTIL_H_

// namespace fs = std::filesystem;

uint64_t TimeSinceEpochMillisec()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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
    srand(TimeSinceEpochMillisec());
    int *arr = new int[n];

    std::iota(arr, arr + n, 0);

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
void MeasureTimeOfFunctionInMilliSeconds(int testDurtionInSecs, T func, Args&&... args)
{
    int counter = 0;
    uint64_t start = TimeSinceEpochMillisec();
    do
    {
        func(std::forward<Args>(args)...); // funkcja do zmierzenia czasu
        counter++;
    } while (TimeSinceEpochMillisec() - start < testDurtionInSecs * 1000);

    int x = (TimeSinceEpochMillisec() - start);
    std::cout << x << std::endl;
    std::cout << counter << std::endl;
    double result = double(x) / double(counter);

    std::cout << result << "ms" << std::endl;
}

Matrix LoadData(std::string const filepath, int const n)
{
    std::ifstream file(filepath);
    Matrix matrix = Matrix(n, n);

    // int counter = 1;
    if (file.is_open())
    {
        std::string line;
        // omijamy pierwsze 6 linii
        for (int i = 0; i < 7; i++)
            std::getline(file, line);

        for (int i = 0; i < n; i++)
        {
            std::getline(file, line);
            // zamiana stringa zawierajacego linie tekstu z pliku na strumien i dzialanie na jak zwyklym pliku
            std::istringstream iss = std::istringstream(line);
            std::string word;
            for (int j = 0; j < n; j++)
            {
                iss >> word;
                matrix.SetValue(i, j, std::stoi(word));
            }
        }
        file.close();
    }

    return matrix;
}

/**
 * Generuje wszystkich sąsiadow dla pemrutacji `arr`.
 */ 
Matrix GenerateNeighbourhood(int arr[], int size)
{
    int size_of_neighbourhood = (size * (size - 1)) / 2;
    Matrix result = Matrix(size_of_neighbourhood, size);
    int cntOfNeighbours = 0;
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            int neighbour[size] = {0};
            std::copy(arr, arr + size, neighbour);
            std::swap(neighbour[i], neighbour[j]);
            result.SetRow(cntOfNeighbours, neighbour);
            cntOfNeighbours++;
        }
    }

    return result;
}

/**
 * Generuje jednego sąsiada. Wartość `numberOfNeighbour` wskazuje, który z kolei ma być to sąsiad.
 */ 
int *GenerateNeighbour(int arr[], int sizeOfArr, int numberOfNeighbour)
{
    int sizeOfNeighbourhood = (sizeOfArr * (sizeOfArr - 1)) / 2;
    int *result = new int[sizeOfArr];

    std::copy(arr, arr + sizeOfArr, result);
    div_t divResult = std::div(numberOfNeighbour, sizeOfArr);
    std::swap(result[divResult.rem], result[divResult.quot]);

    return result;
}

/**
 * Zwraca tablice wartości odległości między miastami w permutacji `arr`.
 */ 
int *GetArrayOfDistances(int arr[], int size, Matrix *distanceMatrix)
{
    int *result = new int[size - 1];
    for (size_t i = 1; i < size; i++)
    {
        int distance = (*distanceMatrix).GetValue(arr[i - 1], arr[i]);
        result[i - 1] = distance;
    }
    return result;
}

int SumOfarray(int *arr, int size)
{
    return std::accumulate(arr, arr + size, 0, std::plus<int>());
}

// nie uzywac
// void UnpackData()
// {
//     system("mkdir ./data/unpacked_first_temp");
//     system("7z e ./data/atsp/ALL_atsp.tar -o./data/unpacked_first_temp");
//     system("7z e ./data/unpacked_first_temp/br17.atsp.gz -o./data/unpacked_ready");
//     system("cd data && rmdir /S /Q unpacked_first_temp");
// }

#endif