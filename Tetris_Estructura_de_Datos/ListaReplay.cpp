#include "ListaReplay.h"

ListaReplay::ListaReplay() {
	primero = nullptr;
	actual = nullptr;
}

ListaReplay::~ListaReplay() {
	reiniciar();
}

void ListaReplay::reiniciar() {
	actual = primero;
	eliminarFuturo();
	if (primero != nullptr) {
		delete primero;
	}
	primero = nullptr;
	actual = nullptr;
}

void ListaReplay::eliminarFuturo() {
	if (actual == nullptr || actual->siguiente == nullptr) {
		return;
	}
	
	// Si se registra un nuevo movimiento despues de deshacer, los estados
	// que estaban por delante ya no pertenecen a la nueva linea de juego.
	NodoReplay* aBorrar = actual->siguiente;
	while (aBorrar != nullptr) {
		NodoReplay* temp = aBorrar;
		aBorrar = aBorrar->siguiente;
		delete temp; // Liberar memoria de cada nodo
	}
	actual->siguiente = nullptr;
}

void ListaReplay::registrarEstado(const Pieza& p, const Tablero& t, const PilaHold& h) {
	eliminarFuturo(); 
	// El replay guarda una copia, no referencias, para que el estado no cambie
	// cuando el juego siga modificando el tablero.
		NodoReplay* nuevo = new NodoReplay();

	nuevo->estado.piezaActual = p;

	nuevo->estado.holdVacio = h.estaVacia();
	nuevo->estado.piezaHold = h.verPieza();
	nuevo->estado.holdBloqueado = !h.puedeIntercambiar();

	for (int fila = 0; fila < 20; fila++) {
		for (int col = 0; col < 10; col++) {
			nuevo->estado.tableroRepleay[fila][col] = t.obtenerCelda(fila, col);
		}
	}

	nuevo->siguiente = nullptr;
	nuevo->anterior = actual;
	
	if (primero == nullptr) {
		primero = nuevo;
	} else {
		actual->siguiente = nuevo;
	}
	
	actual = nuevo;
}

//Se peude desahcer si hay un nodo anterior al actual.
bool ListaReplay::puedeDeshacer() const {
	if (actual != nullptr && actual->anterior != nullptr) {
		return true;
	}
	return false;
}

//Se puede rehacer si hay un nodo siguiente al actual.
bool ListaReplay::puedeRehacer() const {
	if (actual != nullptr && actual->siguiente != nullptr) {
		return true;
	}
	return false;
}

EstadoJuego ListaReplay::deshacer() {
	if (puedeDeshacer()) {
		actual = actual->anterior;
	}
	return actual->estado;
}

EstadoJuego ListaReplay::rehacer() {
	if (puedeRehacer()) {
		actual = actual->siguiente;
	}
	return actual->estado;
}

NodoReplay* ListaReplay::obtenerPrimero() const {
	return primero;
}
