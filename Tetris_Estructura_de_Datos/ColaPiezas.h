#ifndef COLA_PIEZAS_H
#define COLA_PIEZAS_H

#include "Pieza.h"

struct NodoCola
{
	TipoPieza tipo;
	NodoCola *siguiente;
};

class ColaPiezas
{
private:
	NodoCola *frente;
	NodoCola *final;
	int cantidad;

	void generarBolsa();

public:
	ColaPiezas();
	~ColaPiezas();

	void encolar(TipoPieza tipo);
	TipoPieza desencolar();
	TipoPieza verSiguiente(int indice) const;
	void rellenarSiEsNecesario();
};

#endif
