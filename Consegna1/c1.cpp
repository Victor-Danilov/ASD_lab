#include<iostream>
#include<fstream>
#include<vector>
#include<iterator>
#include<sstream>

std::vector<int> vettore; // numeri su cui si lavorerà

void check_file(std::ifstream &file){
    if(!file.is_open()){
        std::cerr<<"File non found!"<<std::endl;
    }
}

// funzione per prendere i dati da file
void take_data(char const* argv){
    std::ifstream data(argv);
    check_file(data);
    
    // Leggo file riga per riga
    std::string line;
    while(std::getline(data, line)){
        std::istringstream ss(line);
        std::string value;

        while(std::getline(ss, value, ',')){
            vettore.push_back(std::stoi(value));
        }
    }
}

// funzione di stampa
void print_data(std::vector<int> &numbers){
    for(int value: numbers)
        std::cout<<value<<", ";
}

void parse_cmd(int argc, char **argv){
    if(argc == 1)
        std::cout<<argv[0]<<"\"nome file\"";
}

int main(int argc, char* argv[])
{
    parse_cmd(argc, argv);
    take_data(argv[1]);
    print_data(vettore);
    return 0;
}

