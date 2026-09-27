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
	bool puedeIntercambiar() const;	
	
	char desapilar();
	char verPieza() const;
	
	void apilar(char tipo);
	void bloquear();
	void desbloquear();
};

#endif
