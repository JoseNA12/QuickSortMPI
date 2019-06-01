#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>          
#include <vector>
#include <algorithm>    // std::copy
//#include "mpi.h" // OpenMPI
#include <omp.h> // OPenMP
#include <chrono> 

using namespace std;
using namespace std::chrono;

/*
    - OpenMPI: sudo apt install libopenmpi-dev (aun no se usa)
    - Guias:
        https://es.wikipedia.org/wiki/OpenMP
        https://bisqwit.iki.fi/story/howto/openmp/
        Libro: https://www.openmp.org/wp-content/uploads/OpenMP4.0.0.Examples.pdf
            Summary: https://www.openmp.org/wp-content/uploads/OpenMP3.0-SummarySpec.pdf
    
*/

// -------------------- Define --------------------
int cant_numerosRandoms = 90000000;
int mul_filas = 255;
// int num_threads = 0;
// ------------------------------------------------

// -------------------- Firmas de funciones --------------------
vector<vector<int>> getListaNumerosRan();
int getParejas(vector<vector<int>> pMatriz);
// -------------------------------------------------------------

/*
    Obtener una matriz de numeros aleatorios.
    La cantidad y longitud de las filas esta definida por el numero de CPU's,
    siendo la longitud multiplicada por 255 (por defecto).
*/
vector<vector<int>> getListaNumerosRan() {
    srand(time(NULL));
    vector<vector<int>> matriz;
    //omp_set_num_threads(1);
    #pragma omp parallel // spawn del conjunto de hilos
    {
        #pragma omp single // un solo hilo ejecuta la instrucción
        matriz.resize(omp_get_num_threads());
        
        #pragma omp barrier // hasta que todos los hilos alcancen esta linea, todos pueden continuar

        #pragma omp for  // dividir entre los hilos las iteraciones del for
        for (int i = 0; i < cant_numerosRandoms; i++) { // cant_numerosRandoms es dividada entre el numero de hilos
            int num = rand() % (mul_filas * omp_get_num_threads()) + 1;
            matriz[omp_get_thread_num()].push_back(num);
        }
    }
    return matriz;
}

int getParejas(vector<vector<int>> pMatriz) {
   return 0;
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        cant_numerosRandoms = atoi(argv[1]);
    }

    auto start = high_resolution_clock::now(); // Get starting timepoint 

    // getParejas(getListaNumerosRan());
    vector<vector<int>> matriz = getListaNumerosRan();
  
    auto stop = high_resolution_clock::now(); // Get ending timepoint 
    auto duration = duration_cast<microseconds>(stop - start); 
   
    cout << "Tiempo: " << (duration.count() * 0.000001) << " segundos" << endl; 
    
    return 0;
}

