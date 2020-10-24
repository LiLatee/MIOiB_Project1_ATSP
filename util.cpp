#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <fstream>
#include <sstream>

uint64_t timeSinceEpochMillisec() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

template <typename T>
void PrintVector(T* const arr, int const size) {
    
    for(T i=0; i<size; i++)
    {
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;
}

int * RandomPermutation(int const n) {
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
void MeasureTimeOfFunctionInMilliSeconds(int testDurtionInSecs, T func, Args&&... args) {
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

int** LoadData(std::string const filepath, int const n)
{
    std::ifstream file(filepath);  
    int** data = new int*[n];
    for (int i = 0; i < n; ++i)
        data[i] = new int[n];

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
            // std::cout << "LINIA: " << counter++ << '\n';
            std::string word;
            for (int j = 0; j < n; j++)
            {
                iss >> word;
                data[i][j] = std::stoi(word);
                // std::cout << word << ' ';
            }
            // std::cout << std::endl;

        }
        file.close();
    }

    return data;
}

template<typename T>
void Print2dSquareArray(T** const arr, int const size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            std::cout << arr[i][j] << "\t";
        std::cout << std::endl;
    }
}

// nie uzywac
void UnpackData() {
    system("mkdir ./data/unpacked_first_temp");
    system("7z e ./data/atsp/ALL_atsp.tar -o./data/unpacked_first_temp");
    system("7z e ./data/unpacked_first_temp/br17.atsp.gz -o./data/unpacked_ready");
    system("cd data && rmdir /S /Q unpacked_first_temp");
}

int main(int argc, char** argv) {
    std::string filepath = argv[1];
    int n = std::stoi(argv[2]);
    std::cout << "PLIK: " << filepath << std::endl;
    std::cout << "LICZBA MIAST: " << n << std::endl;

    int** data = LoadData(filepath=filepath, n=n);
    Print2dSquareArray(data, n);

    // MeasureTimeOfFunctionInMilliSeconds(10, RandomPermutation, 5000);

    // int* randomPermutation = RandomPermutation(5);
    // PrintVector(randomPermutation, 5);
    // delete [] randomPermutation;









} 

