#include "Archivo.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;

// Carga todos los puntajes del archivo a un vector auxiliar
vector<RegistroPuntaje> Archivo::cargarPuntajes()
{
    vector<RegistroPuntaje> lista;
    ifstream archivo("puntajes.txt");
    if (archivo.is_open())
    {
        string nombre;
        int puntaje;
        // Leer hasta que se acabe el archivo
        while (archivo >> nombre >> puntaje)
        {
            RegistroPuntaje reg;
            reg.nombre = nombre;
            reg.puntaje = puntaje;
            lista.push_back(reg);
        }
        archivo.close();
    }
    return lista;
}

void Archivo::guardarPuntaje(string nombre, int puntaje, int metodoOrdenamiento)
{
    // Se carga, agrega y vuelve a escribir el top para mantener el archivo
    // ordenado y limitado a los diez mejores resultados.
    vector<RegistroPuntaje> lista = cargarPuntajes();

    RegistroPuntaje nuevo;
    nuevo.nombre = nombre;
    nuevo.puntaje = puntaje;
    lista.push_back(nuevo);

    if (metodoOrdenamiento == 0)
    {
        insertionSort(lista);
    }
    else
    {
        mergeSort(lista, 0, (int)lista.size() - 1);
    }

    ofstream archivo("puntajes.txt");
    if (archivo.is_open())
    {
        int limite = lista.size() < 10 ? lista.size() : 10;
        for (int i = 0; i < limite; i++)
        {
            archivo << lista[i].nombre << " " << lista[i].puntaje << "\n";
        }
        archivo.close();
    }
}

void Archivo::insertionSort(vector<RegistroPuntaje> &v)
{
    int n = static_cast<int>(v.size());
    for (int i = 1; i < n; i++)
    {
        // Cada registro se inserta en la posicion correcta de la parte ya ordenada.
        RegistroPuntaje actual = v[i];
        int j = i - 1;
        // Se usa < en lugar de > para ordenar de mayor a menor
        while (j >= 0 && v[j].puntaje < actual.puntaje)
        {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = actual;
    }
}

void Archivo::merge(vector<RegistroPuntaje> &v, int inicio, int medio, int fin)
{
    // Se mezclan dos mitades ordenadas eligiendo primero el puntaje mayor.
    // Mismo constructor de vector con iteradores del profe
    vector<RegistroPuntaje> izquierda(v.begin() + inicio, v.begin() + medio + 1);
    vector<RegistroPuntaje> derecha(v.begin() + medio + 1, v.begin() + fin + 1);

    int i = 0, j = 0, k = inicio;

    while (i < (int)izquierda.size() && j < (int)derecha.size())
    {
        if (izquierda[i].puntaje >= derecha[j].puntaje)
        {
            v[k] = izquierda[i];
            i++;
        }
        else
        {
            v[k] = derecha[j];
            j++;
        }
        k++;
    }

    while (i < (int)izquierda.size())
    {
        v[k] = izquierda[i];
        i++;
        k++;
    }

    while (j < (int)derecha.size())
    {
        v[k] = derecha[j];
        j++;
        k++;
    }
}

void Archivo::mergeSort(vector<RegistroPuntaje> &v, int inicio, int fin)
{
    if (inicio >= fin)
        return;
    // Divide hasta tener elementos individuales y luego los combina ordenados.
    int medio = inicio + (fin - inicio) / 2;
    mergeSort(v, inicio, medio);
    mergeSort(v, medio + 1, fin);
    merge(v, inicio, medio, fin);
}

//Metodo para obtener los tiempos de los ordenamientos, no se usa como tal en el juego
void Archivo::probarOrdenamientos()
{
    int tamanos[4] = {10, 100, 1000, 10000};
    int repeticiones = 5;

    cout << "\nPRUEBA DE ORDENAMIENTOS\n";
    cout << "Cantidad\tInsertion Sort\tMergeSort\n";

    srand(12345);

    for (int t = 0; t < 4; t++)
    {
        int cantidad = tamanos[t];

        long long totalInsertion = 0;
        long long totalMerge = 0;

        for (int prueba = 0; prueba < repeticiones; prueba++)
        {
            vector<RegistroPuntaje> datos;

            // Generar datos temporales
            for (int i = 0; i < cantidad; i++)
            {
                RegistroPuntaje registro;

                registro.nombre = "Prueba";
                registro.puntaje = rand() % 100000;

                datos.push_back(registro);
            }

            // Los dos algoritmos reciben los mismos datos
            vector<RegistroPuntaje> datosInsertion = datos;
            vector<RegistroPuntaje> datosMerge = datos;

            // Insertion Sort
            auto inicioInsertion =
                chrono::high_resolution_clock::now();

            insertionSort(datosInsertion);

            auto finInsertion =
                chrono::high_resolution_clock::now();

            // MergeSort
            auto inicioMerge =
                chrono::high_resolution_clock::now();

            mergeSort(datosMerge, 0, datosMerge.size() - 1);

            auto finMerge =
                chrono::high_resolution_clock::now();

            long long tiempoInsertion =
                chrono::duration_cast<chrono::microseconds>(finInsertion - inicioInsertion).count();

            long long tiempoMerge =
                chrono::duration_cast<chrono::microseconds>(finMerge - inicioMerge).count();

            totalInsertion += tiempoInsertion;
            totalMerge += tiempoMerge;
        }

        long long promedioInsertion =
            totalInsertion / repeticiones;

        long long promedioMerge =
            totalMerge / repeticiones;

        cout << cantidad << "\t\t"
             << promedioInsertion << " us\t\t"
             << promedioMerge << " us\n";
        cout << endl;
    }
    cout << endl<< endl<< endl;
}
