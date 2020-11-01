#include <iostream>
#include "matrix.h"
#include "util.h"

int simple_heur(std::string filepath, int nOfCities){
    const int nOfNeighbours = (nOfCities * (nOfCities - 1)) / 2;
    int *currentPermutation = RandomPermutation(nOfCities);
    std::cout<<*currentPermutation;
    return 1 ;

}

int main(){
    int i = simple_heur("data/unpacked_ready/br17.atsp",17);
    return 0;
}