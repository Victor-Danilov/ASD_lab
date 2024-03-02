#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <iostream>


// compilazione: g++ lezione1-1-insertion_sort-simple.c
//
// Obiettivi:

// Lanciare con
// > ./a.out 10
// > ./a.out 100

// > time ./a.out 1000
// > time ./a.out 10000...

// grafico con andamento lunghezza array vs tempo
// progressione quadratica?

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void insertion_sort(int *A, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i - 1; j >= 0; j--) {
            if (A[j] > A[j + 1]) {
                swap(&A[j], &A[j + 1]);
            } else {
                break;
            }
        }
    }
}

int main(int argc, char **argv) {
    int n, i;
    int *A;

    /// controllo argomenti
    if (argc != 2) {
        printf("Usage: %s dim-array\n", argv[0]);
        return 1;
    }

    /// parsing argomento
    n = atoi(argv[1]);

    /// allocazione array
    // A = (int*) malloc(n*sizeof(int));

    A = new int[n];

    /// inizializzazione array
    for (i = 0; i < n; i++) {
        A[i] = n - i;
    }

    /// input
    // printf("L'input e':\n");
    // for (i = 0; i < n; i++) {
    //     printf("%d ", A[i]);
    // }
    // printf("\n");

    const std::clock_t c_start = std::clock();
    /// sort
    insertion_sort(A, n);

    const std::clock_t c_end = std::clock();

    /// output
    // printf("L'output e':\n");
    // for (i = 0; i < n; i++) {
    //     printf("%d ", A[i]);
    // }
    printf("\n");

    long double time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    std::cout << "CPU time used: " 
            << time_elapsed_ms 
            << " ms\n";

    return 0;
}
