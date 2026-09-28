#include "ColaEventos.h"

ColaEventos::ColaEventos() {
    frente = nullptr;
}

ColaEventos::~ColaEventos() {
    vaciar();
}

//Insertar respetando el orden de prioridad: O(n)
// Se inserta DESPUES de todos los nodos con tiempo >= al nuevo,
// para que, entre iguales, se mantenga el orden de llegada (FIFO).
void ColaEventos::encolar(Evento e) {
    NodoEvento* nuevo = new NodoEvento{e, nullptr};

    //Menor tiempoActivacion = mayor prioridad (va al frente)
    if (frente == nullptr || frente->evento.tiempoActivacion > e.tiempoActivacion) {
        nuevo->siguiente = frente;
        frente = nuevo;
        return;
    }

    // Se avanza hasta el ultimo evento que debe ir antes o al mismo tiempo.
    // El <= conserva el orden de llegada cuando dos eventos coinciden.
    NodoEvento* actual = frente;
    while (actual->siguiente != nullptr && actual->siguiente->evento.tiempoActivacion <= e.tiempoActivacion) {
        actual = actual->siguiente;
    }
    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
}

//Extraer el elemento de mayor prioridad (el frente): O(1)
Evento ColaEventos::desencolar() {
    if (frente == nullptr) {
        return {0, 0.0f}; //Falso/Vacio si la cola esta vacia
    }
    NodoEvento* aBorrar = frente;
    Evento valorExtraido = aBorrar->evento;
    frente = frente->siguiente;
    delete aBorrar;
    return valorExtraido;
}

//Ver el frente sin extraerlo
Evento ColaEventos::verFrente() const {
    if (frente == nullptr) {
        return {0, 0.0f};
    }
    return frente->evento;
}

bool ColaEventos::estaVacia() const {
    return frente == nullptr;
}

void ColaEventos::vaciar() {
    while (frente != nullptr) {
        NodoEvento* aBorrar = frente;
        frente = frente->siguiente;
        delete aBorrar;
    }
}
