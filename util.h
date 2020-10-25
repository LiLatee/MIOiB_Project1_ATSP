#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <fstream>
#include <sstream>
// #include <filesystem>
#include "squared_matrix.h"
#ifndef UTIL_H_
#define UTIL_H_

// namespace fs = std::filesystem;


uint64_t timeSinceEpochMillisec() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


int* randomPermutation(int const n) {
    srand(timeSinceEpochMillisec());
    int *arr = new int[n];

    // inicjalizacja tablicy 
    for (int i = 0; i < n; i++)
        arr[i] = i;

    // losowanie permutacji
    int temp;
    for (int i = 0; i < n; i++)
    {
        temp = arr[n-1-i]; // zawsze zamieniamy ostatnia wartosc w kazdej iteracji przesuwajac sie w lewo
        int randomIndex = rand() % (n-i); // (n-i), bo po usunieciu elementu przesuwamy index w lewo   
        arr[n-1-i] = arr[randomIndex]; // zamiana ostatniej wartosci na wylosowana
        arr[randomIndex] = temp;
    }

    return arr;
}
template <typename T, typename... Args>
void measureTimeOfFunctionInMilliSeconds(int testDurtionInSecs, T func, Args&&... args) {
    int counter = 0;
    uint64_t start = timeSinceEpochMillisec();
    do {
            int* randomPermutation = func(std::forward<Args>(args)...); // funkcja do zmierzenia
            delete [] randomPermutation;
            counter++;
    } while (timeSinceEpochMillisec() - start < testDurtionInSecs*1000);


    int x = (timeSinceEpochMillisec()-start);
    std::cout << x << std::endl;
    std::cout << counter<< std::endl;
    double result = double(x)/double(counter);

    std::cout << result << "ms" << std::endl;


}
SquaredMatrix loadData(std::string const filepath, int const n)
{
    std::ifstream file(filepath); 
    SquaredMatrix matrix = SquaredMatrix(n); 

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
            std::istringstream iss = std::istringstream(line);
            std::string word;
            for (int j = 0; j < n; j++)
            {
                iss >> word;
                matrix.setValue(i,j,std::stoi(word));
            }

        }
        file.close();
    }

    return matrix;
}

// nie uzywac
void UnpackData() {
    system("mkdir ./data/unpacked_first_temp");
    system("7z e ./data/atsp/ALL_atsp.tar -o./data/unpacked_first_temp");
    system("7z e ./data/unpacked_first_temp/br17.atsp.gz -o./data/unpacked_ready");
    system("cd data && rmdir /S /Q unpacked_first_temp");
}


#endif