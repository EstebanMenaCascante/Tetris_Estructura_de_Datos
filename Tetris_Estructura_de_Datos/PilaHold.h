#ifndef PILA_HOLD_H
#define PILA_HOLD_H

#include "Pieza.h"

class PilaHold {
private:
	char* elemento;
	bool bloqueado;
	
public:
	PilaHold();
	~PilaHold();
	
	bool estaVacia() const;
	char verPieza() const;
	void apilar(char tipo);
	char desapilar();
	
	bool puedeIntercambiar() const;
	void bloquear();
	void desbloquear();
};

#endif
