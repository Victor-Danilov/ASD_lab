#include<iostream>
#include<fstream>
#include<vector>
#include<iterator>

std::vector<int> numbers; // numeri su cui si lavorerà

// funzione per prendere i dati da file
void data(char const* argv){
    std::ifstream data(argv);
    std::copy(std::istream_iterator<int>(data), std::istream_iterator<int>(), std::back_inserter(numbers));
}

// funzione di stampa
void print(std::vector<int> numbers){
    for(; numbers.begin()!=numbers.end(); ++numbers.begin())
        std::cout<<*numbers.begin()<<", ";
}

void parse_cmd(int argc, char **argv){
    if(argc == 1)
        std::cout<<argv[0]<<"\"nome file\"";
}

int main(int argc, char* argv[])
{
    parse_cmd(argc, argv);
    data(argv[1]);
    print(numbers);
    return 0;
}

