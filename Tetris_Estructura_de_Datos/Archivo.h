#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>
#include <vector>

struct RegistroPuntaje {
    std::string nombre;
    int puntaje;
};

class Archivo {
public:
    std::vector<RegistroPuntaje> cargarPuntajes();
    void guardarPuntaje(std::string nombre, int puntaje, int metodoOrdenamiento);

    // Algoritmos de ordenamiento (públicos para poder usarlos en el menú si se quiere)
    void insertionSort(std::vector<RegistroPuntaje>& v);
    void mergeSort(std::vector<RegistroPuntaje>& v, int inicio, int fin);
	void merge(std::vector<RegistroPuntaje>& v, int inicio, int medio, int fin);
	
private:
    
};

#endif
