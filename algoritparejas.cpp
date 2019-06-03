#include <stdio.h>          /* printf, scanf, puts, NULL */
#include <stdlib.h>         /* srand, rand */
#include <time.h>           /* time */
#include <iostream>          
#include <vector>
#include <bits/stdc++.h>    /* tuple */
#include <chrono>           /* high_resolution_clock, duration_cast */

//#include "mpi.h" // OpenMPI
#include <omp.h> // OPenMP


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
int cant_numerosRandoms = 9000;//90000000;
int tope = 1020;
int num_hilos = 4;
// ------------------------------------------------

// -------------------- Firmas de funciones --------------------
vector<vector<int>> getListaNumerosRan();
vector<tuple<int, int>> getParejas(vector<vector<int>> pMatriz);
// -------------------------------------------------------------

/*
    Obtener una matriz de numeros aleatorios.
    La cantidad y longitud de las filas esta definida por el numero de CPU's,
    siendo la longitud multiplicada por 255 (por defecto).
*/
vector<vector<int>> getListaNumerosRan() {
    srand(time(NULL));
    vector<vector<int>> matriz;

    omp_set_num_threads(num_hilos);

    #pragma omp parallel // spawn del conjunto de hilos
    {
        #pragma omp single // un solo hilo ejecuta la instrucción
        matriz.resize(omp_get_num_threads());
        
        #pragma omp barrier // hasta que todos los hilos alcancen esta linea, todos pueden continuar

        #pragma omp for  // dividir entre los hilos las iteraciones del for
        for (int i = 0; i < cant_numerosRandoms; i++) { // cant_numerosRandoms es dividada entre el numero de hilos
            matriz[omp_get_thread_num()].push_back(rand() % tope + 1);
        }
    }
    return matriz;
}

vector<tuple<int, int>> getParejas(vector<vector<int>> pMatriz) {
    vector<tuple<int, int>> parejas;

    omp_set_num_threads(num_hilos);

    #pragma omp parallel // spawn del conjunto de hilos
    {
        int pos_actual = 0;     // Indica el elemento que se esta analizando
        int pos_busqueda = 0;   // Recorre la lista cada vez que se busca un elemento
        int num_pivote = 0;
        int num_match = 0;
        int tamanio_vector = pMatriz[omp_get_thread_num()].size();
        int num_hilo = omp_get_thread_num();

        while (pos_actual < tamanio_vector) {
            pos_busqueda = 0;
            num_pivote = pMatriz[num_hilo][pos_actual];

            if (num_pivote != -1 ) { // no buscar -1's

                while (pos_busqueda < tamanio_vector) { 
                    num_match = pMatriz[num_hilo][pos_busqueda];

                    if (pos_busqueda != pos_actual && num_pivote == num_match) { // 1er cond: no hacer match él mismo
                        #pragma omp critical // un hilo a la vez
                        {
                            parejas.push_back(make_tuple(num_pivote, num_match));
                        }
                        pMatriz[num_hilo][pos_actual] = -1;
                        pMatriz[num_hilo][pos_busqueda] = -1;
                    }
                    pos_busqueda++;
                }
            }
            pos_actual++;
        }
    }
    return parejas;
}

void ejecutarAlgoritmo() {
    vector<tuple<int, int>> result;

    auto inicio = high_resolution_clock::now(); // punto inicial de tiempo
    result = getParejas(getListaNumerosRan());
    auto fin = high_resolution_clock::now(); // punto final de tiempo 
    auto duration = duration_cast<microseconds>(fin - inicio);

    cout << "Tiempo de ejecución: " << (duration.count() * 0.000001) << " segundos" << endl;
    cout << "Numero de hilos empleados: " << num_hilos << endl;
    cout << "Total de números generados: " << cant_numerosRandoms << endl;
    cout << "Total de parejas: " << (int) result.size() << endl;
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        cant_numerosRandoms = atoi(argv[1]);
    }
    if (argc > 2) {
        num_hilos = atoi(argv[2]);
    }

    ejecutarAlgoritmo();
    return 0;
}

