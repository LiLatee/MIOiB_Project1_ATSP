#include <iostream>
#include "matrix.h"
#include "util.h"

int simple_heur(std::string filepath, int nOfCities){
    Matrix distanceMatrix = LoadData(filepath, nOfCities);

    int *finallPath = RandomPermutation(nOfCities);
    int icities = 1;
    int distanceValue = 0;
    int currentCity = finallPath[0];
    int destinationCity = finallPath[1];
    int currentDistance = distanceMatrix.GetValue(currentCity,destinationCity);
    int possibleDistance = 0;

    while (icities < nOfCities){
        for( int i = icities; i<nOfCities;i++){ // sprawdzamy odległość do najbliższego miasta
            possibleDistance = distanceMatrix.GetValue(currentCity,finallPath[i]);
            if( currentDistance > possibleDistance){
                currentDistance = possibleDistance;
                destinationCity = i;

            }   
            // std::cout<<"for"<<i<<std::endl;   
        }
        // std::cout<<"distance: "<<currentDistance<<std::endl;    

        distanceValue =distanceValue+ currentDistance;
        std::swap(finallPath[icities],finallPath[destinationCity]);
        icities ++;
        currentCity = destinationCity;
        destinationCity = finallPath[icities];
        currentDistance = distanceMatrix.GetValue(currentCity,destinationCity);
        // std::cout<<"while"<<icities<<std::endl;         

    }
    
    // for(int i=1;i<nOfCities;i++){
    //     std::cout<<finallPath[i]<<std::endl;
    //     std::cout<<"DISTANCE "<<distanceMatrix.GetValue(i,i-1)<<std::endl;
        
    // }
    return distanceValue ;

}

int main(){
    // int i = simple_heur("data/unpacked_ready/br17.atsp",17);
    // std::cout<<i<<std::endl;
    MeasureTimeOfFunctionInMilliSeconds(1, simple_heur, "data/unpacked_ready/br17.atsp",17);
    return 0;

}