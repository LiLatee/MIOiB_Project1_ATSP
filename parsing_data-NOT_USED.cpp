#include<iostream>
#include<fstream>
#include<string>
#include<vector>

std::string FILE_NAME = "data/unpacked_ready/br17.atsp"; // path to the file with data

#define SIZE 17  // for now we give the size in the global variable

int main(){

    int distances [SIZE][SIZE]; // finall matrix with distances


    
    std::ifstream file_with_distances;
    file_with_distances.open(FILE_NAME);
    std::string line;
    std::string matrix_string;
    std::vector<std::string> file_vec = {};
    // std::vector<int> num_vector = {};
    
    int line_int = 0;

    if(file_with_distances.is_open()){
        while(getline(file_with_distances,line)){
            if(line_int < 7){
                file_vec.push_back(line);
            }
            else{
                matrix_string.append(line);
                
            }
            line_int ++;
        }
    
    }
    std::string value = "";


    int counter_val = 0;
    int num_val = 0;

    for (char c : matrix_string){
        
        if (c == ' ' && value.length()>0){
            num_val =  std::stoi(value);
            distances[counter_val/SIZE][counter_val%SIZE]=num_val;
            value = "";
            counter_val++;
        }
        if (c != ' ' && std::isdigit(c)){
            value = value + c;
        }  
    }
    distances[SIZE-1][SIZE-1]=std::stoi(value); 

    
    for(int i = 0; i<SIZE; i++){
            for(int j = 0; j<SIZE; j++){
            std::cout<<distances[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    
    return 0;
}