#ifndef PILA_HOLD_H
#define PILA_HOLD_H

#include "Pieza.h"

class PilaHold {
private:
	TipoPieza* elemento;
	bool bloqueado;
	
public:
	PilaHold();
	~PilaHold();
	
	bool estaVacia() const;
	TipoPieza verPieza() const;
	void apilar(TipoPieza tipo);
	TipoPieza desapilar();
	
	bool puedeIntercambiar() const;
	void bloquear();
	void desbloquear();
};

#endif
