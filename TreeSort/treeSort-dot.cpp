#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <time.h>
using namespace std;

// compilazione: g++ lezione4-6-quicksort.c
//
// Obiettivo:

// prove con array ordinato crescente, decrescente --> numero di confronti?
// ./a.out 1024 -d=16

// prove con array random --> numero di confronti?
// ./a.out 1024 -d=16 -t=100

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int max_dim = 0;
int ntests = 1;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0; /// dimensione dell'array

/// file di output per grafo
ofstream output_graph;

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        cout << A[j] << " ";
    }
    cout << "\n";
}

void print_array_graph(int *A, int p, int r, string s, int pivot) {
    /// prepara il disegno dell'array A ed il suo contenuto dall'indice a all'indice b inclusi
    /// usa la stringa c per stampare il nome del nodo
    /// se pivot = 1 -> colora di rosso lo sfondo

    /// uso codice HTML per creare una tabella con il contenuto dell'array

    // return ;

    output_graph << s << p << "_" << r << " [label=<" << endl
                 << "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" ";
    if (pivot)
        output_graph << "bgcolor=\"#ff8080\"";
    output_graph << "> <TR>" << endl;
    for (int j = p; j <= r; j++) {
        output_graph << "<TD>" << A[j] << "</TD>" << endl;
    }
    output_graph << "</TR> </TABLE>>];" << endl;
}

// Tree Sort
/****************************************************************************************************************************/

// Dobbiamo definire il dato "nodo" dell'albero
struct Nodo {
    int key;
    struct Nodo *left, *right;
};

// Dibbiamo definire la funzione per creare un nuovo nodo
struct Nodo *nuovoNodo(int valore) // crea il nuovo nodo
{
    struct Nodo* nuovoNodo = new Nodo; // crea un nodo vuoto
    // if(!nuovoNodo) { // gestisco l'allocazione della memoria
    //     printf("Memoria esaurita\n");
    //     return NULL;
    // }
    nuovoNodo -> key = valore; // assegno il valore al nodo
    nuovoNodo -> left = nuovoNodo -> right = NULL; // inizializzo i puntatori a NULL

    return nuovoNodo; // restituisco il nodo
}

// Dobbiamo definire l'inserimento nell'albero
Nodo* addNodeValue(Nodo* nodo, int valore) // inserisce il valore nel nodo
{
    if(!nodo) return nuovoNodo(valore); // se il nodo è vuoto, lo creo

    if(valore < nodo -> key) // se il valore è minore del nodo corrente
        nodo -> left = addNodeValue(nodo -> left, valore); // inserisco il valore nel sottoalbero sinistro
    else if(valore > nodo -> key) // se il valore è maggiore del nodo corrente
        nodo -> right = addNodeValue(nodo -> right, valore); // inserisco il valore nel sottoalbero destro

    return nodo; // restituisco il nodo
}

// Dobbiamo deifnire la funzione per attravereamento in ordine e l'immagazzinamento nell'array
void inOrder(Nodo* nodo, int arr[], int &i) // attraversamento in ordine
{
    if(nodo) {
        inOrder(nodo -> left, arr, i); // attraverso il sottoalbero sinistro
        ct_read++;
        arr[i++] = nodo -> key; // immagazzino il valore nell'array
        inOrder(nodo -> right, arr, i); // attraverso il sottoalbero destro
    }
}

// Algoritmo di sorting
void treeSort(int arr[], int n){
    struct Nodo *root = NULL;

    root = addNodeValue(root, arr[rand() % n]);
    for (int i = 1; i < n; i++)
    {
        ct_read++;
        addNodeValue(root, arr[i]);
    }
    // organizzo l'array
    int i = 0;
    inOrder(root, arr, i);
}

/****************************************************************************************************************************/


int parse_cmd(int argc, char **argv) {
    /// controllo argomenti
    if (argc < 2) {
        cout << "Usage: " << argv[0] << "%s max-dim [Options]\n";
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

    /// parsing argomento
    max_dim = atoi(argv[1]);

    for (int i = 2; i < argc; i++) {
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

    // init random
    srand((unsigned)time(NULL));

    if (ndiv > 1)
        cout << "Dim_array,N_tes,min_read,avg_read,max_read,n*n,n*log(n)\n";

    // printf("Parametri: max-dim %d, d %d, t %d, verbose %d\n",max_dim,ndiv,ntests,details);

    //// inizio ciclo per calcolare ndiv dimensioni di array crescenti
    for (n = max_dim / ndiv; n <= max_dim; n += max_dim / ndiv) {

        int cmp_min = -1;
        int cmp_max = -1;
        long cmp_avg = 0;

        int read_min = -1;
        int read_max = -1;
        long read_avg = 0;

        //// lancio ntests volte per coprire diversi casi di input random
        for (test = 0; test < ntests; test++) {

            /// inizializzazione array: numeri random con range dimensione array
            for (i = 0; i < n; i++) {
                //A[i] = i;
                // A[i] = n - i;
                A[i] = rand() % (n * 50);
            }

            if (details) {
                cout << "creato array di dimensione " << n << "\n";
                print_array(A, n);
            }

            ct_read = 0;

            /// algoritmo di sorting
            //quick_sort(A, 0, n - 1);
            treeSort(A, n);


            if (details) {
                cout << "Output:\n";
                print_array(A, n);
            }

            /// statistiche
            read_avg += ct_read;
            if (read_min < 0 || read_min > ct_read)
                read_min = ct_read;
            if (read_max < 0 || read_max < ct_read)
                read_max = ct_read;
        }

        if (ndiv > 1)
            cout << n << "," << ntests << "," << read_min << "," << (0.0 + read_avg) / ntests << "," << read_max << "," << 0.0 + n * n << "," << 0.0 + n * log(n) / log(2) << "\n";
    }

    delete[] A;

    return 0;
}
