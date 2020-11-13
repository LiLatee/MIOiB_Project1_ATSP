#include <iostream>
#include <time.h>       /* time */
#include "matrix.h"
#include "util.cpp"
#include <stdlib.h>




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
            // *finallPath = *newPath;
        }
        // PrintVector(newPath,nOfCities);
        // std::cout<<std::endl;
        // counter+=1;
    }    
    // std::cout<<counter<<std::endl;
    return bestDistanceValue;


}

int getCostDiffForNeighbour(int *arr,Matrix distanceMatrix, Pair<int> swappedIndexes, int nOfCities){
    //first index new value
    // [i-1][i] oraz [i][i+1]
    int currentCost = 0 ;
    int oldCost = 0;

    // current 
    // PrintVector(arr,nOfCities);
    // std::cout<<"first: "<<swappedIndexes.first<<" second: "<<swappedIndexes.second<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<arr[(swappedIndexes.first-1)%nOfCities]<<std::endl;
    // std::cout<<arr[(swappedIndexes.first)%nOfCities]<<std::endl;
    // std::cout<<arr[(swappedIndexes.first+1)%nOfCities]<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<arr[(swappedIndexes.second-1)%nOfCities]<<std::endl;
    // std::cout<<arr[(swappedIndexes.second)%nOfCities]<<std::endl;
    // std::cout<<arr[(swappedIndexes.second+1)%nOfCities]<<std::endl;
    // exit(0);

    currentCost =currentCost+ distanceMatrix.GetValue( arr[(swappedIndexes.first-1)%nOfCities],arr[swappedIndexes.first])
                + distanceMatrix.GetValue( arr[swappedIndexes.first],arr[(swappedIndexes.first+1)%nOfCities]);
    currentCost =currentCost+ distanceMatrix.GetValue( arr[(swappedIndexes.second-1)%nOfCities],arr[swappedIndexes.second])
                + distanceMatrix.GetValue( arr[swappedIndexes.second],arr[(swappedIndexes.second+1)%nOfCities]);     
    // old 

    oldCost = oldCost+ distanceMatrix.GetValue( arr[(swappedIndexes.first-1)%nOfCities],arr[swappedIndexes.second])
                + distanceMatrix.GetValue( arr[swappedIndexes.second],arr[(swappedIndexes.first+1)%nOfCities]);
    oldCost = oldCost+ distanceMatrix.GetValue( arr[(swappedIndexes.second-1)%nOfCities],arr[swappedIndexes.first])
                + distanceMatrix.GetValue( arr[swappedIndexes.first],arr[(swappedIndexes.second+1)%nOfCities]);   

    // std::cout<<"currentCost "<<currentCost<<std::endl;
    // std::cout<<"oldCost "<<oldCost<<std::endl;
    // std::cout<<"diff "<<currentCost-oldCost<<std::endl;
    return currentCost-oldCost;
    

    //second index new value
} 

Pair<int> getRandomNeighbour(int *arr, int nOfCities){
    
    int randomIndexFirst = rand() % nOfCities;
    int randomIndexSecond= 0;
    do{
    randomIndexSecond = rand() % nOfCities;

    }while(randomIndexSecond==randomIndexFirst);

    std::swap(arr[randomIndexFirst], arr[randomIndexSecond]);
    Pair swappedIndexes = Pair(randomIndexFirst,randomIndexSecond);
    // std::cout<<"randomIndexFirst "<<randomIndexFirst<<std::endl;
    // std::cout<<"randomIndexSecond "<<randomIndexSecond<<std::endl;


    return swappedIndexes;
   
}

int RandomWalk(Matrix distanceMatrix, int nOfCities, int timeInS){

    time_t current_time = time(NULL);

    int bestDistanceValue = 9999999;
    int *finallPath = RandomPermutation(nOfCities);
    int newDistanceValue = 0;
    int oldDistanceValue = 0; 

    for(int i =0; i<nOfCities-1;i++){
            int cost = distanceMatrix.GetValue( finallPath[i],finallPath[i+1]);
            oldDistanceValue = oldDistanceValue + cost;
        }
    oldDistanceValue = oldDistanceValue + distanceMatrix.GetValue( finallPath[nOfCities- 1],finallPath[0]);
    // std::cout<<"OLD DIST: "<<oldDistanceValue<<std::endl;

    while(time(NULL) < current_time+ timeInS){
        Pair swappedIndexes = getRandomNeighbour(finallPath,nOfCities);

        // newDistanceValue = oldDistanceValue + getCostDiffForNeighbour(finallPath,distanceMatrix,swappedIndexes,nOfCities);
        newDistanceValue=0;
        for(int i =0; i<nOfCities-1;i++){
                int cost = distanceMatrix.GetValue( finallPath[i],finallPath[i+1]);
                newDistanceValue = newDistanceValue + cost;
            }
        newDistanceValue = newDistanceValue + distanceMatrix.GetValue( finallPath[nOfCities- 1],finallPath[0]);

        // std::cout<<"OLD DIST: "<<oldDistanceValue<<std::endl;
        // std::cout<<"NEW DIST: "<<newDistanceValue<<std::endl;

        if (newDistanceValue < bestDistanceValue){
            bestDistanceValue = newDistanceValue;
        }
        oldDistanceValue =newDistanceValue;
        // exit(0);
    }    
    PrintVector(finallPath,nOfCities);
    newDistanceValue=0;
    for(int i =0; i<nOfCities-1;i++){
        int cost = distanceMatrix.GetValue( finallPath[i],finallPath[i+1]);
        newDistanceValue = newDistanceValue + cost;
    }

    std::cout<<"CURRENT " <<newDistanceValue<<std::endl;
    return bestDistanceValue;
    
}


// int main(int argc, char **argv)
// {
//     srand(TimeSinceEpochMillisec());
//     std::string filepath = argv[1];
//     std::cout << "PLIK: " << filepath << std::endl;

//     int nOfCities;
//     Matrix distanceMatrix = LoadData(filepath, nOfCities);

//     int result = RandomWalk(distanceMatrix, nOfCities,10);
//     std::cout << "RandomWalk: " << result << std::endl;


//     // int firstIndex=0;
//     // int secondIndex=0;

//     // int *arr = new int[1,2,3,4];
//     // PrintVector(arr,4);
//     // Pair swapped = getRandomNeighbour(arr,4);
//     // std::cout<<"FIRS : "<<swapped.first<<" SECOND: "<<swapped.second<<std::endl;
//     // PrintVector(arr,4);

//     return 0;
// }
