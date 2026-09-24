#include "Pieza.h"
#include "raylib.h"

struct Coordenada
{
	int x;
	int y;
};

const Coordenada FORMAS[7][4][4] =
	{
		{
			{{0, 1}, {1, 1}, {2, 1}, {3, 1}},
			{{2, 0}, {2, 1}, {2, 2}, {2, 3}},
			{{0, 2}, {1, 2}, {2, 2}, {3, 2}},
			{{1, 0}, {1, 1}, {1, 2}, {1, 3}}},

		{
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}}},

		{
			{{1, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{1, 0}, {1, 1}, {2, 1}, {1, 2}},
			{{0, 1}, {1, 1}, {2, 1}, {1, 2}},
			{{1, 0}, {0, 1}, {1, 1}, {1, 2}}},

		{
			{{1, 0}, {2, 0}, {0, 1}, {1, 1}},
			{{1, 0}, {1, 1}, {2, 1}, {2, 2}},
			{{1, 1}, {2, 1}, {0, 2}, {1, 2}},
			{{0, 0}, {0, 1}, {1, 1}, {1, 2}}},

		{
			{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
			{{2, 0}, {1, 1}, {2, 1}, {1, 2}},
			{{0, 1}, {1, 1}, {1, 2}, {2, 2}},
			{{1, 0}, {0, 1}, {1, 1}, {0, 2}}},

		{
			{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {1, 2}},
			{{0, 1}, {1, 1}, {2, 1}, {2, 2}},
			{{1, 0}, {1, 1}, {0, 2}, {1, 2}}},

		{
			{{2, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{1, 0}, {1, 1}, {1, 2}, {2, 2}},
			{{0, 1}, {1, 1}, {2, 1}, {0, 2}},
			{{0, 0}, {1, 0}, {1, 1}, {1, 2}}}};

Pieza crearPieza(TipoPieza tipo)
{
	Pieza nueva;

	nueva.tipo = tipo;
	nueva.x = 3;
	nueva.y = 0;
	nueva.rotacion = 0;

	return nueva;
}

int obtenerXBloque(const Pieza &pieza, int bloque)
{
	int tipo = (int)pieza.tipo;

	return pieza.x +
		   FORMAS[tipo][pieza.rotacion][bloque].x;
}

int obtenerYBloque(const Pieza &pieza, int bloque)
{
	int tipo = (int)pieza.tipo;

	return pieza.y +
		   FORMAS[tipo][pieza.rotacion][bloque].y;
}

Color obtenerColorPieza(TipoPieza tipo)
{
	switch (tipo)
	{
	case I:
		return SKYBLUE;

	case O:
		return YELLOW;

	case T:
		return PURPLE;

	case S:
		return GREEN;

	case Z:
		return RED;

	case J:
		return BLUE;

	case L:
		return ORANGE;
	}

	return WHITE;
}

void dibujarPieza(
	const Pieza &pieza,
	int xTablero,
	int yTablero,
	int tamCelda)
{
	Color color = obtenerColorPieza(pieza.tipo);

	for (int bloque = 0; bloque < 4; bloque++)
	{
		int columna = obtenerXBloque(pieza, bloque);
		int fila = obtenerYBloque(pieza, bloque);

		int x = xTablero + columna * tamCelda;
		int y = yTablero + fila * tamCelda;

		DrawRectangle(
			x + 2,
			y + 2,
			tamCelda - 4,
			tamCelda - 4,
			color);
	}
}

bool posicionValida(
	const Pieza &pieza,
	const Tablero &tablero)
{
	for (int bloque = 0; bloque < 4; bloque++)
	{
		int columna = obtenerXBloque(pieza, bloque);
		int fila = obtenerYBloque(pieza, bloque);

		if (columna < 0 || columna >= 10)
		{
			return false;
		}

		if (fila >= 20)
		{
			return false;
		}

		if (fila < 0)
		{
			return false;
		}

		if (tablero.obtenerCelda(fila, columna) != 0)
		{
			return false;
		}
	}

	return true;
}

bool moverPieza(
	Pieza &pieza,
	int movimientoX,
	int movimientoY,
	const Tablero &tablero)
{
	Pieza nuevaPosicion = pieza;

	nuevaPosicion.x += movimientoX;
	nuevaPosicion.y += movimientoY;

	if (posicionValida(nuevaPosicion, tablero))
	{
		pieza = nuevaPosicion;

		return true;
	}

	return false;
}

bool rotarPieza(
	Pieza &pieza,
	const Tablero &tablero)
{
	Pieza nuevaRotacion = pieza;

	nuevaRotacion.rotacion++;

	if (nuevaRotacion.rotacion >= 4)
	{
		nuevaRotacion.rotacion = 0;
	}

	if (posicionValida(nuevaRotacion, tablero))
	{
		pieza = nuevaRotacion;

		return true;
	}

	return false;
}
