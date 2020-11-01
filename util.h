#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <numeric>
#include <regex>
#include "matrix.h"

#ifndef UTIL_H_
#define UTIL_H_


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
void MeasureTimeOfFunctionInMilliSeconds(int testDurtionInSecs, std::string funcName, T func, Args &&... args)
{
    int counter = 0;
    uint64_t start = TimeSinceEpochMillisec();
    do
    {
        func(std::forward<Args>(args)...); // funkcja do zmierzenia czasu
        counter++;
    } while (TimeSinceEpochMillisec() - start < testDurtionInSecs * 1000);

    int x = (TimeSinceEpochMillisec() - start);
    // std::cout << x << std::endl;
    // std::cout << counter << std::endl;
    double result = double(x) / double(counter);

    std::cout << "Czas dla: " << funcName << "\t" << result << " ms" << std::endl;
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
            if (std::regex_match(line, std::regex("DIMENSION:[ ]*[0-9].")))
            {
                std::smatch m;
                std::regex_search(line, m, std::regex("[0-9]."));
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
    std::cout << "Nie udało się wczytać pliku!" << std::endl;
    exit(0);
}

struct Pair {
    int first;
    int second;

    Pair() {
        this->first = 0;
        this->second = 0;
    }
    Pair(int first, int second) {
        this->first = first;
        this->second = second;
    }

    int& operator[](int index) {
        if (index == 0) return this->first;
        else if(index == 1) return this->second;
        else {
            std::cout << "Object pair has only 0 and 1 indexes.";
            exit(0);
        }
    }


};
/**
 * Generuje wszystkich sąsiadow dla pemrutacji `arr`.
 */
Matrix GenerateWhole_2OPT(int arr[], int size, Pair* pairs)
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
            pairs[cntOfNeighbours] = Pair(i, j);
            cntOfNeighbours++;    
        }
    }

    return result;
}

/**
 * Zwraca tablice wartości odległości między miastami w permutacji `arr`.
 */
int *GetArrayOfDistances(int arr[], int size, Matrix *distanceMatrix)
{
    int *result = new int[size];
    for (size_t i = 1; i < size; i++)
    {
        int distance = (*distanceMatrix).GetValue(arr[i - 1], arr[i]);
        result[i - 1] = distance;
    }

    // dodanie odleglosci laczacej ostatni z pierwszym
    int distance = (*distanceMatrix).GetValue(arr[size - 1], arr[0]);
    result[size - 1] = distance;
    return result;
}

/**
 * Zwraca tablice wartości odległości między miastami w permutacji `neighbourPermutation`. 
 * Ale nie oblicza całego wyniku od nowa tylko same zmiany względem odległości rodzica `parentDistancesArray`.
 * Gdy zamieniamy 2 elementy ze są w permutacji to zmieniają się 4 wartości w tablicy odległości.
 * np. z permutacji 12345 do 14325 zamiana 2 z 4 powoduje że zmieniają się odległości 
 * z 1 do 2 --> z 1 do 4
 * z 2 do 3 --> z 4 do 3
 * z 3 do 4 --> z 3 do 2
 * z 4 do 5 --> z 2 do 5
 * więc tylko te 4 wartości w tablicy odległości rodzica zmieniamy
 * 
 */
int *GetArrayOfDistancesUsingParentPermutation(int neighbourPermutation[], int parentDistancesArray[], Matrix *distanceMatrix, Pair swappedIndexes, int nOfCities)
{
    int *result = new int[nOfCities];
    std::copy(parentDistancesArray, parentDistancesArray + nOfCities, result);
    // iterujemy po 2 zmienionych idenksach odpowiadajacych indeksom zamienionych elemntow
    for (int i = 0; i < 2; i++)
    {
        int newValue;

        // indeks elementu przed zamienionym elementem
        int firstChangedValueIndex = swappedIndexes[i] - 1;
        // jeżeli pierwszy zamieniony element to pierwsza wartość permtutacji to jego zmiana wpływa na ostatni element permutacji
        // bo osttani element łączy sie z pierwszym
        if (firstChangedValueIndex == -1)
            firstChangedValueIndex = nOfCities - 1;
        newValue = distanceMatrix->GetValue(neighbourPermutation[firstChangedValueIndex], neighbourPermutation[swappedIndexes[i]]);
        result[firstChangedValueIndex] = newValue;

        // indeks elementu po zamienionym elemencie
        int secondChangedValueIndex = swappedIndexes[i] + 1;
        // to samo co wyżej, ale sprawdzamy czy to nie jest osttani element, który wpływ ana pierwszy
        if (secondChangedValueIndex == nOfCities)
            secondChangedValueIndex = 0;
        newValue = distanceMatrix->GetValue(neighbourPermutation[swappedIndexes[i]], neighbourPermutation[secondChangedValueIndex]);

        result[swappedIndexes[i]] = newValue;
    }
    return result;
}

int SumOfarray(int arr[], int size)
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