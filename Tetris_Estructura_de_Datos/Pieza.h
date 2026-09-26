#ifndef PIEZA_H
#define PIEZA_H

#include "Tablero.h"

struct Pieza
{
	char tipo;

	int x;
	int y;

	int rotacion;
};


Pieza crearPieza(char tipo);

int obtenerIndice(char letra);

int obtenerXBloque(const Pieza &pieza, int bloque);

int obtenerYBloque(const Pieza &pieza, int bloque);

void dibujarPieza(const Pieza &pieza, int xTablero, int yTablero,int tamCelda);

bool posicionValida(const Pieza &pieza,const Tablero &tablero);

bool moverPieza(Pieza &pieza, int movimientoX, int movimientoY, const Tablero &tablero);

bool rotarPieza(Pieza &pieza, const Tablero &tablero);

#endif
