#ifndef PIEZA_H
#define PIEZA_H

#include "Tablero.h"

enum TipoPieza
{
	I,
	O,
	T,
	S,
	Z,
	J,
	L
};

struct Pieza
{
	TipoPieza tipo;

	int x;
	int y;

	int rotacion;
};

// Crear una pieza
Pieza crearPieza(TipoPieza tipo);

// Obtener posicion de cada bloque
int obtenerXBloque(const Pieza &pieza, int bloque);

int obtenerYBloque(const Pieza &pieza, int bloque);

// Dibujar pieza
void dibujarPieza(
	const Pieza &pieza,
	int xTablero,
	int yTablero,
	int tamCelda);

// --------------------------------------------------
// Movimiento y colisiones
// --------------------------------------------------

bool posicionValida(
	const Pieza &pieza,
	const Tablero &tablero);

bool moverPieza(
	Pieza &pieza,
	int movimientoX,
	int movimientoY,
	const Tablero &tablero);

bool rotarPieza(
	Pieza &pieza,
	const Tablero &tablero);
#endif
