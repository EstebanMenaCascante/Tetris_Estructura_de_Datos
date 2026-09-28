#include "PilaHold.h"

PilaHold::PilaHold() {
	elemento = nullptr;
	bloqueado = false;
}

PilaHold::~PilaHold() {
	if (elemento != nullptr) {
		delete elemento;
	}
}

bool PilaHold::estaVacia() const {
	return elemento == nullptr;
}

char PilaHold::verPieza() const {
	if (elemento != nullptr) {
		return *elemento;
	}
	return 'I';
}

void PilaHold::apilar(char tipo) {
	// Solo existe una pieza en Hold: la primera operacion reserva memoria y
	// las siguientes reemplazan el valor guardado.
	if (elemento == nullptr) {
		elemento = new char(tipo);
	} else {
		*elemento = tipo;
	}
}

char PilaHold::desapilar() {
	// Se devuelve la pieza y se libera el unico nodo dinamico de la pila.
	if (elemento != nullptr) {
		char tipo = *elemento;
		delete elemento;
		elemento = nullptr;
		return tipo;
	}
	return 'I';
}

bool PilaHold::puedeIntercambiar() const {
	return !bloqueado;
}

void PilaHold::bloquear() {
	bloqueado = true;
}

void PilaHold::desbloquear() {
	bloqueado = false;
}
