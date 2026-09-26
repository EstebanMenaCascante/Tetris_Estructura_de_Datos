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

void ColaPiezas::encolar(char tipo)
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

char ColaPiezas::desencolar()
{
	if (frente == nullptr)
		return 'I';

	NodoCola *eliminar = frente;
	char tipo = eliminar->tipo;

	frente = frente->siguiente;
	if (frente == nullptr)
	{
		final = nullptr;
	}

	delete eliminar;
	cantidad--;

	return tipo;
}

char ColaPiezas::verSiguiente(int indice) const
{
	NodoCola *actual = frente;
	
	for (int i = 0; i < indice && actual != nullptr; i++)
	{
		actual = actual->siguiente;
	}
	if (actual != nullptr)
		return actual->tipo;
	return 'I';
}

void ColaPiezas::generarBolsa()
{
	char letras[7] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
	bool usadas[7] = {false, false, false, false, false, false, false}; // un poco de flags
	int agregadas = 0; // contador de piezas agregadas

	// Las 7 piezas sin repetir
	while (agregadas < 7)
	{
		int aleatorio = rand() % 7; // del 0 al 6

		if (usadas[aleatorio] == false)
		{
			usadas[aleatorio] = true;
			encolar(letras[aleatorio]);
			agregadas++;
		}
	}
}

void ColaPiezas::rellenarSiEsNecesario()
{
	// Rellena la cola cuando quedan 3 piezas o menos
	if (cantidad <= 3)
	{
		generarBolsa();
	}
}
