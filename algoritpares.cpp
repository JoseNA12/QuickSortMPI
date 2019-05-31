#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>          
#include <list>
//#include "mpi.h" // OpenMPI
#include <omp.h> // OPenMP

using namespace std;

/*
    - OpenMPI: sudo apt install libopenmpi-dev
    - Guias:
    https://es.wikipedia.org/wiki/OpenMP
    https://bisqwit.iki.fi/story/howto/openmp/
    https://bisqwit.iki.fi/story/howto/openmp/#ExampleInitializingATableInParallelMultipleThreads
    https://www.openmp.org/wp-content/uploads/OpenMP3.0-SummarySpec.pdf
*/

// -------------------- Define --------------------
#define cant_numerosRandoms 1000000
// ------------------------------------------------

// -------------------- Firmas de funciones --------------------
list<int> getListaNumerosRan();
// -------------------------------------------------------------


list<int> getListaNumerosRan() {
    list<int> numerosRandoms;
    srand (time(NULL));
    cout << "Empiezo" << endl;
    int i;
    int cant_iteraciones_x_hilo;
    

    #pragma omp parallel private(numerosRandoms, i) // spawn del conjunto de hilos
    {
        #pragma omp single // hago que un hilo distribuya la cantidad de numeros a generar segun los CPU's de la maquina
        cant_iteraciones_x_hilo = (int) cant_numerosRandoms / omp_get_max_threads(); // division entera
        omp_lock_t lock[cant_iteraciones_x_hilo];
        
        #pragma omp barrier // sincronizar los hilos para que se esperen entre ellos

        printf("\n[Proc: %d], [Cant. iteraciones: %d]", omp_get_thread_num(), cant_iteraciones_x_hilo);

        #pragma omp for  // dividir entre los hilos las iteraciones del for
        for (i = 0; i <= cant_iteraciones_x_hilo; i++) {
            numerosRandoms.push_back(rand() % 10 + 1);
        }
    }
    cout << "\nListo" << endl;
    return numerosRandoms;
}

int main ()
{
    list<int> listaNumsRan = getListaNumerosRan();
    printf("\nLen: %d\n", (int) listaNumsRan.size());
    //prueba();
    
}