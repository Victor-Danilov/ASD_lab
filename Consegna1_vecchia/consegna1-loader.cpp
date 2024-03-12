#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// compilazione: g++ consegna1-loader.c
//

// Il programma carica il file data.txt contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;
int ct_write = 0;

int max_dim = 0;
int ntests = 0;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0; /// dimensione dell'array

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}


void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
    /// aggiorno contatore globale di swap
    ct_read++;
}

int partition(int *A, int p, int r) {

    /// copia valori delle due meta p..q e q+1..r
    ct_read++;
    int x = A[r];
    int i = p - 1;

    for (int j = p; j < r; j++) {
        ct_cmp++;
        ct_read++;
        if (A[j] <= x) {
            i++;
            ct_read++;
            ct_read++;
            swap(A[i], A[j]);
        }
    }
    ct_read++;
    ct_read++;
    swap(A[i + 1], A[r]);

    return i + 1;
}

void quick_sort(int *A, int p, int r) {
    /// gli array L e R sono utilizzati come appoggio per copiare i valori: evita le allocazioni nella fase di merge
    if (p < r) {
        int q = partition(A, p, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

void Insertion_sort(int* A, int n){
    int i;
    int j;
    for(i = 0; i < n; i++){
        for(j = i - 1; j >= 0; j--){

            ct_read++; // lettura A[i]
            ct_read++; // lettura A[i+1]

            if(A[j] > A[j+1]) {
                swap(A[j], A[j+1]);

                ct_write++; // scrittura su A[i]
                ct_write++; // scrittura su A[i+1]
            }  
            else break;
        }
    }
}

void merge(int *A, int p, int q, int r, int *L, int *R) {

    /// copia valori delle due meta p..q e q+1..r
    int i = 0;
    int j = 0;
    int k = 0;

    for (i = 0; i < q - p + 1; i++)
        ct_write++;
        L[i] = A[p + i];
    L[i] = 1000000; /// un numero grande

    for (i = 0; i < r - q; i++)
        ct_write++;
        R[i] = A[q + 1 + i];
    R[i] = 1000000; /// un numero grande

    //// dettagli
    if (details) {
        cout << "Array L (" << p << " .. " << q << "): ";
        print_array(L, q - p + 1);
        cout << "Array R (" << q + 1 << " .. " << r << "): ";
        print_array(R, r - q);
    }

    i = 0; /// usato per gestire array L
    j = 0; /// usato per gestire array R

    for (k = p; k <= r; k++) {
        ct_read++;
        ct_read++;
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
            ct_write++;
        } else {
            A[k] = R[j];
            j++;
            ct_write++;

        }
    }
}

void merge_sort(int *A, int p, int r, int *L, int *R) {
    /// gli array L e R sono utilizzati come appoggio per copiare i valori: evita le allocazioni nella fase di merge
    if (p < r) {
        int q = (p + r) / 2;
        merge_sort(A, p, q, L, R);
        merge_sort(A, q + 1, r, L, R);
        merge(A, p, q, r, L, R);
        if (details)
            print_array(A, n);
    }
}

int parse_cmd(int argc, char **argv) {

    /// parsing argomento
    max_dim = atoi(argv[1]);

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " max-dim [Options]\n";
        cout << "   max-dim: specifica la massima dimensione n del problema\n";
        cout << "Options:\n";
        cout << "  -d=<int>: Specifica quali dimensioni n del problema vengono lanciate in sequenza [default: 1] \n";
        cout << "            n = k * max-dim / d, k=1 .. d\n";
        cout << "  -t=<int>: Specifica quanti volte viene lanciato l'algoritmo per una specifica dimensione n [default: 1]\n";
        cout << "            Utile nel caso in cui l'input viene inizializzato in modo random\n";
        cout << "  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n";
        cout << "  -graph: creazione file di dot con il grafo dell'esecuzione (forza d=1 t=1)\n";
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'd')
            ndiv = atoi(argv[i] + 3);
        if (argv[i][1] == 't')
            ntests = atoi(argv[i] + 3);
        if (argv[i][1] == 'v')
            details = 1;
        if (argv[i][1] == 'g') {
            graph = 1;
            ndiv = 1;
            ntests = 1;
        }
    }

    return 0;
}


int main(int argc, char **argv) {
    int i, test;
    int *A;
    int *B; /// buffer per visualizzazione algoritmo

    if (parse_cmd(argc, argv))
        return 1;

    /// allocazione array
    A = new int[max_dim];

    n = max_dim;
    
    int *L; /// usato come buffer di appoggio
    int *R; /// usato come buffer di appoggio

    L = new int[max_dim + 1];
    R = new int[max_dim + 1];

    ifstream input_data;
    input_data.open("data.txt");

    int write_min = -1;
    int write_max = -1;
    long write_avg = 0;

    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;


    //// lancio ntests volte per coprire diversi casi di input random
    printf("ntest,read_min,read_avg,read_max,write_min,write_avg,write_max\n");
    for (test = 0; test < ntests; test++) {

        /// inizializzazione array: numeri random con range dimensione array
        for (i = 0; i < n; i++) {
            char comma;
            input_data >> A[i];
            input_data >> comma;
        }

        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;

        /// algoritmo di sorting
        //merge_sort(A, 0, n - 1, L, R);
        Insertion_sort(A, max_dim);


        if (details) {
            printf("Output:\n");
            print_array(A, n);
        }


        /// statistiche
        read_avg += ct_read;
        write_avg += ct_write;

        if (read_min < 0 || read_min > ct_read)
            read_min = ct_read;
        if (read_max < 0 || read_max < ct_read)
            read_max = ct_read;

        if (write_min < 0 || write_min > ct_write)
            write_min = ct_write;
        if (write_max < 0 || write_max < ct_write)
            write_max = ct_write;
        // printf("%d,%d,%d \n", test, ct_read, ct_write);
        printf("%d,%d,%.1f,%d,%d,%.1f,%d\n", test,
                   read_min, (0.0 + read_avg) / ntests, read_max,
                   write_min, (0.0 + write_avg) / ntests, write_max);
    }


    printf("\nntest,read_min,read_avg,read_max,write_min,write_avg,write_max\n");
    printf("%d,%d,%.1f,%d\n",
           ntests,
           read_min, (0.0 + read_avg) / ntests, read_max,
           write_min, (0.0 + write_avg) / ntests, write_max);

    delete[] A;

    return 0;
}
