#include "ColaPiezas.h"
#include <cstdlib>
#include <ctime>

ColaPiezas::ColaPiezas()
{
	frente = nullptr;
	final = nullptr;
	cantidad = 0;
	srand(time(0));
	generarBolsa();
}

ColaPiezas::~ColaPiezas()
{
	while (frente != nullptr)
	{
		desencolar();
	}
}

void ColaPiezas::encolar(TipoPieza tipo)
{
	NodoCola *nuevo = new NodoCola();
	nuevo->tipo = tipo;
	nuevo->siguiente = nullptr;

	if (frente == nullptr)
	{
		frente = nuevo;
		final = nuevo;
	}
	else
	{
		final->siguiente = nuevo;
		final = nuevo;
	}
	cantidad++;
}

TipoPieza ColaPiezas::desencolar()
{
	if (frente == nullptr)
		return I;

	NodoCola *eliminar = frente;
	TipoPieza tipo = eliminar->tipo;

	frente = frente->siguiente;
	if (frente == nullptr)
	{
		final = nullptr;
	}

	delete eliminar;
	cantidad--;

	return tipo;
}

TipoPieza ColaPiezas::verSiguiente(int indice) const
{
	NodoCola *actual = frente;
	
	for (int i = 0; i < indice && actual != nullptr; i++)
	{
		actual = actual->siguiente;
	}
	if (actual != nullptr)
		return actual->tipo;
	return I;
}

void ColaPiezas::generarBolsa()
{
	TipoPieza bolsa[7] = {I, O, T, S, Z, J, L};

	for (int i = 6; i > 0; i--)
	{
		int j = rand() % (i + 1);
		TipoPieza temp = bolsa[i];
		bolsa[i] = bolsa[j];
		bolsa[j] = temp;
	}

	for (int i = 0; i < 7; i++)
	{
		encolar(bolsa[i]);
	}
}

void ColaPiezas::rellenarSiEsNecesario()
{
	if (cantidad <= 3)
	{
		generarBolsa();
	}
}
