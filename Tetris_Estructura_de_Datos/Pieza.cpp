#include "Pieza.h"
#include "raylib.h"

// ---------------------------------------------------------
// Una coordenada representa la posicion de un bloque
// dentro de la pieza.
// ---------------------------------------------------------

struct Coordenada
{
	int x;
	int y;
};

// ---------------------------------------------------------
// Formas de las piezas
//
// [pieza][rotacion][bloque]
//
// 7 piezas
// 4 rotaciones
// 4 bloques
// ---------------------------------------------------------

const Coordenada FORMAS[7][4][4] =
	{
		// =====================================================
		// I
		// =====================================================
		{
			{{0, 1}, {1, 1}, {2, 1}, {3, 1}},
			{{2, 0}, {2, 1}, {2, 2}, {2, 3}},
			{{0, 2}, {1, 2}, {2, 2}, {3, 2}},
			{{1, 0}, {1, 1}, {1, 2}, {1, 3}}},

		// =====================================================
		// O
		// =====================================================
		{
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {2, 1}}},

		// =====================================================
		// T
		// =====================================================
		{
			{{1, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{1, 0}, {1, 1}, {2, 1}, {1, 2}},
			{{0, 1}, {1, 1}, {2, 1}, {1, 2}},
			{{1, 0}, {0, 1}, {1, 1}, {1, 2}}},

		// =====================================================
		// S
		// =====================================================
		{
			{{1, 0}, {2, 0}, {0, 1}, {1, 1}},
			{{1, 0}, {1, 1}, {2, 1}, {2, 2}},
			{{1, 1}, {2, 1}, {0, 2}, {1, 2}},
			{{0, 0}, {0, 1}, {1, 1}, {1, 2}}},

		// =====================================================
		// Z
		// =====================================================
		{
			{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
			{{2, 0}, {1, 1}, {2, 1}, {1, 2}},
			{{0, 1}, {1, 1}, {1, 2}, {2, 2}},
			{{1, 0}, {0, 1}, {1, 1}, {0, 2}}},

		// =====================================================
		// J
		// =====================================================
		{
			{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{1, 0}, {2, 0}, {1, 1}, {1, 2}},
			{{0, 1}, {1, 1}, {2, 1}, {2, 2}},
			{{1, 0}, {1, 1}, {0, 2}, {1, 2}}},

		// =====================================================
		// L
		// =====================================================
		{
			{{2, 0}, {0, 1}, {1, 1}, {2, 1}},
			{{1, 0}, {1, 1}, {1, 2}, {2, 2}},
			{{0, 1}, {1, 1}, {2, 1}, {0, 2}},
			{{0, 0}, {1, 0}, {1, 1}, {1, 2}}}};

// ---------------------------------------------------------
// Crear una pieza
// ---------------------------------------------------------

Pieza crearPieza(TipoPieza tipo)
{
	Pieza nueva;

	nueva.tipo = tipo;

	// Posicion inicial aproximadamente centrada
	nueva.x = 3;
	nueva.y = 0;

	nueva.rotacion = 0;

	return nueva;
}

// ---------------------------------------------------------
// Obtener posicion X de uno de los bloques
// ---------------------------------------------------------

int obtenerXBloque(const Pieza &pieza, int bloque)
{
	int tipo = (int)pieza.tipo;

	return pieza.x +
		   FORMAS[tipo][pieza.rotacion][bloque].x;
}

// ---------------------------------------------------------
// Obtener posicion Y de uno de los bloques
// ---------------------------------------------------------

int obtenerYBloque(const Pieza &pieza, int bloque)
{
	int tipo = (int)pieza.tipo;

	return pieza.y +
		   FORMAS[tipo][pieza.rotacion][bloque].y;
}

// ---------------------------------------------------------
// Obtener color de la pieza
// ---------------------------------------------------------

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

// ---------------------------------------------------------
// Dibujar pieza
// ---------------------------------------------------------

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

// ---------------------------------------------------------
// Verificar si la posicion de una pieza es valida
// ---------------------------------------------------------

bool posicionValida(
	const Pieza &pieza,
	const Tablero &tablero)
{
	for (int bloque = 0; bloque < 4; bloque++)
	{
		int columna = obtenerXBloque(pieza, bloque);
		int fila = obtenerYBloque(pieza, bloque);

		// Revisar limites laterales
		if (columna < 0 || columna >= 10)
		{
			return false;
		}

		// Revisar limite inferior
		if (fila >= 20)
		{
			return false;
		}

		// Revisar limite superior
		if (fila < 0)
		{
			return false;
		}

		// Revisar si la celda del tablero esta ocupada
		if (tablero.obtenerCelda(fila, columna) != 0)
		{
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------
// Mover pieza
// ---------------------------------------------------------

bool moverPieza(
	Pieza &pieza,
	int movimientoX,
	int movimientoY,
	const Tablero &tablero)
{
	Pieza nuevaPosicion = pieza;

	nuevaPosicion.x += movimientoX;
	nuevaPosicion.y += movimientoY;

	// Solamente hacemos el movimiento si es valido
	if (posicionValida(nuevaPosicion, tablero))
	{
		pieza = nuevaPosicion;

		return true;
	}

	return false;
}

// ---------------------------------------------------------
// Rotar pieza
// ---------------------------------------------------------

bool rotarPieza(
	Pieza &pieza,
	const Tablero &tablero)
{
	Pieza nuevaRotacion = pieza;

	nuevaRotacion.rotacion++;

	// Hay solamente 4 rotaciones: 0, 1, 2 y 3
	if (nuevaRotacion.rotacion >= 4)
	{
		nuevaRotacion.rotacion = 0;
	}

	// Si la nueva orientacion cabe, rotamos
	if (posicionValida(nuevaRotacion, tablero))
	{
		pieza = nuevaRotacion;

		return true;
	}

	// Si no cabe, no se rota
	return false;
}
