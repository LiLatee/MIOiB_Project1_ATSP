#ifndef ALGORITHMS_HELPERS_FUNCS_CPP_
#define ALGORITHMS_HELPERS_FUNCS_CPP_

#include <random>
#include "matrix.h"
#include "util.cpp"



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