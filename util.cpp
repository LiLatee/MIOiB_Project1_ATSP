#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>

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

int main() {
    MeasureTimeOfFunctionInMilliSeconds(10, RandomPermutation, 5000);
    // int* randomPermutation = RandomPermutation(5);
    // PrintVector(randomPermutation, 5);
    // delete [] randomPermutation;
    // for(int i=0; i<10;i++)
    // {
    //     std::vector<int> randomPermutation = RandomPermutation(5);
    //     PrintVector(randomPermutation);
    // }


} 

