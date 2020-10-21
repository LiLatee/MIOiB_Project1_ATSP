#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>

uint64_t timeSinceEpochMillisec() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

template <typename T>
void PrintVector(std::vector<T> const vec) {
    for(auto i =vec.begin(); i!=vec.end(); i++)
    {
        std::cout << *i << " ";
    }

    std::cout << std::endl;
}


std::vector<int> RandomPermutation(int const n) {
    srand(timeSinceEpochMillisec());
    std::vector<int> vec;

    // inicjalizacja wektora 
    for (int i = 0; i < n; i++)
    {
        vec.push_back(i);
    }

    // losowanie permutacji
    for (int i = 0; i < n; i++)
    {
        int randomIndex = rand() % (n-i); // (n-i), bo po usunieciu elementu przesuwamy index w lewo
        vec.push_back(vec[randomIndex]); // dodajemy wylosownay element na koniec
        vec.erase(vec.begin() + randomIndex); // // usuwamy wylosowany element
    }

    // PrintVector(vec);

    return vec;
}

template <typename T, typename... Args>
double MeasureTimeOfFunctionInMilliSeconds(int testDurtionInSecs, T func, Args&&... args) {
    int counter = 0;
    uint64_t start = timeSinceEpochMillisec();
    do {
            std::vector<int> randomPermutation = func(std::forward<Args>(args)...); // funkcja do zmierzenia
            counter++;
    } while (timeSinceEpochMillisec() - start < testDurtionInSecs*1000);


    int x = (timeSinceEpochMillisec()-start);
    std::cout << x << std::endl;
    std::cout << counter<< std::endl;
    double result = double(x)/double(counter);
    return result;

}

int main() {
    double timeInMilliSeconds = MeasureTimeOfFunctionInMilliSeconds(5, RandomPermutation, 5);
    std::cout << timeInMilliSeconds << std::endl;
    // std::vector<int> randomPermutation = RandomPermutation(5);
    // PrintVector(randomPermutation);

} 

