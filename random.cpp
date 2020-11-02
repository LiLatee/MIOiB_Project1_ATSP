#include <iostream>
#include <ctime>
#include "matrix.h"
#include "util.h"

const int SEED = 42;


int Random(Matrix distanceMatrix, int nOfCities, int timeInS)
{
    // int counter = 1;
    time_t current_time = time(NULL);

    int bestDistanceValue = 0;
    int *finallPath = RandomPermutation(nOfCities);

    for(int i =0; i<nOfCities-1;i++){
        int cost = distanceMatrix.GetValue( finallPath[i],finallPath[i+1]);
        bestDistanceValue = bestDistanceValue + cost;
    }
    bestDistanceValue= bestDistanceValue+ distanceMatrix.GetValue( finallPath[nOfCities- 1],finallPath[0]);

    while(time(NULL) < current_time+ timeInS){
        int *newPath = RandomPermutation(nOfCities);
        int newDistanceValue =  0;
        for(int i =0; i<nOfCities-1;i++){
            int cost = distanceMatrix.GetValue( newPath[i],newPath[i+1]);
            newDistanceValue = newDistanceValue + cost;
        }
        newDistanceValue = newDistanceValue + distanceMatrix.GetValue( newPath[nOfCities- 1],newPath[0]);

        if(newDistanceValue < bestDistanceValue){
            bestDistanceValue = newDistanceValue;
            *finallPath = *newPath;
        }
        PrintVector(newPath,nOfCities);
        std::cout<<std::endl;
        // counter+=1;
    }    
    // std::cout<<counter<<std::endl;
    return bestDistanceValue;


}




int main(int argc, char **argv)
{
    srand((SEED));
    std::string filepath = argv[1];
    std::cout << "PLIK: " << filepath << std::endl;

    int nOfCities;
    Matrix distanceMatrix = LoadData(filepath, nOfCities);

    int result = Random(distanceMatrix, nOfCities,1);
    std::cout << "Random: " << result << std::endl;



}
