#include<iostream>
#include<fstream>
#include<vector>
#include<iterator>
#include<sstream>
#include"algorithms.hh"

/* ATTENZIONE: algoritmi definiti nel file "algorithms.hh" */

int ct_read = 0;

std::vector<int> vettore; // numeri su cui si lavorerà

/**
 * @brief Funzione che controlla se il file esiste e lo apre nel caso ci fosse
 * 
 * @param file file di lavorazione
 */
void check_file(std::ifstream &file){
    if(!file.is_open()){
        std::cerr<<"File non found!"<<std::endl;
    }
}


/**
 * @brief Funzione che legge i dati dal file
 * 
 * @param argv nome del file su cui lavorare
 */
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

/**
 * @brief Funzione di stampa
 * 
 * @param numbers elementi da stampare
 */
void print_data(std::vector<int>& numbers){
    for(int value: numbers)
        std::cout<<value<<", ";
}


/**
 * @brief Analisi dell'input
 * 
 * @param argc 
 * @param argv 
 */
void parse_cmd(int argc, char **argv){
    if(argc == 1)
        std::cout<<argv[0]<<"\"nome file\"";
}

/**
 * @brief Funzione main
 * 
 * @param argc numero argomento
 * @param argv arogmento
 * @return int 
 */
int main(int argc, char* argv[])
{
    parse_cmd(argc, argv);
    take_data(argv[1]);
    //print_data(vettore);

    std::vector<int> prova{1,2,3,4,5};

    std::cout<<prova.size();
    //Insertion_sort(vettore);
    //std::cout<<"Stampa\n";
    //print_data(vettore);
    return 0;
}
