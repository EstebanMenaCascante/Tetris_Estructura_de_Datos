#include "Archivo.h"
#include <fstream>
#include <iostream>

using namespace std;

//Carga todos los puntajes del archivo a un vector auxiliar
vector<RegistroPuntaje> Archivo::cargarPuntajes() {
    vector<RegistroPuntaje> lista;
    ifstream archivo("puntajes.txt");
    if (archivo.is_open()) {
        string nombre;
        int puntaje;
        //Leer hasta que se acabe el archivo
        while (archivo >> nombre >> puntaje) {
            RegistroPuntaje reg;
            reg.nombre = nombre;
            reg.puntaje = puntaje;
            lista.push_back(reg);
        }
        archivo.close();
    }
    return lista;
}

void Archivo::guardarPuntaje(string nombre, int puntaje, int metodoOrdenamiento) {
    vector<RegistroPuntaje> lista = cargarPuntajes();

    RegistroPuntaje nuevo;
    nuevo.nombre = nombre;
    nuevo.puntaje = puntaje;
    lista.push_back(nuevo);

    if (metodoOrdenamiento == 0) {
        insertionSort(lista);
    } else {
        mergeSort(lista, 0, (int)lista.size() - 1);
    }

    ofstream archivo("puntajes.txt");
    if (archivo.is_open()) {
        int limite = lista.size() < 10 ? lista.size() : 10;
        for (int i = 0; i < limite; i++) {
            archivo << lista[i].nombre << " " << lista[i].puntaje << "\n";
        }
        archivo.close();
    }
}

void Archivo::insertionSort(vector<RegistroPuntaje>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 1; i < n; i++) {
        RegistroPuntaje actual = v[i];
        int j = i - 1;
        // Se usa < en lugar de > para ordenar de mayor a menor
        while (j >= 0 && v[j].puntaje < actual.puntaje) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = actual;
    }
}

void Archivo::merge(vector<RegistroPuntaje>& v, int inicio, int medio, int fin) {
    //Mismo constructor de vector con iteradores del profe
    vector<RegistroPuntaje> izquierda(v.begin() + inicio, v.begin() + medio + 1);
    vector<RegistroPuntaje> derecha(v.begin() + medio + 1, v.begin() + fin + 1);

    int i = 0, j = 0, k = inicio;
    
    while (i < (int)izquierda.size() && j < (int)derecha.size()) {
        if (izquierda[i].puntaje >= derecha[j].puntaje) {
            v[k] = izquierda[i];
            i++;
        } else {
            v[k] = derecha[j];
            j++;
        }
        k++;
    }

    while (i < (int)izquierda.size()) { 
        v[k] = izquierda[i]; 
        i++; 
        k++; 
    }
    
    while (j < (int)derecha.size()) { 
        v[k] = derecha[j]; 
        j++; 
        k++; 
    }
}

void Archivo::mergeSort(vector<RegistroPuntaje>& v, int inicio, int fin) {
    if (inicio >= fin) return; 
    int medio = inicio + (fin - inicio) / 2;
    mergeSort(v, inicio, medio);
    mergeSort(v, medio + 1, fin);
    merge(v, inicio, medio, fin);
}
