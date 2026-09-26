#include "ListaReplay.h"

ListaReplay::ListaReplay() {
	primero = nullptr;
	actual = nullptr;
}

ListaReplay::~ListaReplay() {
	actual = primero; 
	eliminarFuturo();
	if (primero != nullptr) {
		delete primero;
	}
}

void ListaReplay::eliminarFuturo() {
	if (actual == nullptr || actual->siguiente == nullptr) {
		return;
	}
	
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

bool ListaReplay::puedeDeshacer() const {
	if (actual != nullptr && actual->anterior != nullptr) {
		return true;
	}
	return false;
}

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
