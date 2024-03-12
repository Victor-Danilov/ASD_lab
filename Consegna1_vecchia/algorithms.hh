#ifndef ALGORITHMS_HH_INCLUDE_GUARD
#define ALGORITHMS_HH_INCLUDE_GUARD 1

#include<vector>

/* COUNTING SORT START*/
// /**
//  * @brief Funzione che trova l'elemento massimo nella sequenza di elementi
//  * 
//  * @param numbers vettore di elementi
//  * @return int 
//  */
// int max(std::vector<int>& numbers){
//     int max{0};
//     for(int i=0; i<=numbers.size(); ++i){
//         if(numbers[i] > max){
//             max = numbers[i];
//         }
//     }
//     return max;
// }

// std::vector<int> elem_occ(auto& numbers, auto& C){

// }

// void counting_Sort(auto& vettore){
//     std::vector<int> C;
//     for(int i=0; i<=vettore.size(); ++i){
//         while()
//     }

//     delete C;
// }

/* COUNTING SORT END*/

/* INSERTION SORT START*/

void swap(int& a, int& b){
    int tmp = a;
    a = b;
    b = tmp;
}

void Insertion_sort(std::vector<int>& A){
    int i;
    int j;
    for(i = 0; i < A.size(); ++i){
        for(j = i - 1; j >= 0; --j){
            if(A[i] > A[i+1]) swap(A[i], A[i+1]);
            else break;
        }
    }
}

/* INSERTION SORT END*/

/* Quick Sort Start */


/* Quick Sort End */


#endif // ALGORITHMS_HH_INCLUDE_GUARD