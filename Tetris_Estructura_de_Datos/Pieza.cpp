#include "Pieza.h"
#include "raylib.h"

struct Coordenada
{
	int x;
	int y;
};

// FORMAS[pieza][rotacion][bloque] guarda la coordenada relativa de cada uno
// de los cuatro bloques que forman una pieza.
// El primer indice sigue el orden I, O, T, S, Z, J, L; el segundo representa
// las cuatro rotaciones posibles (0 a 3). Por eso no se calcula la rotacion
// con formulas: se consulta directamente la forma correspondiente.
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

// Convierte la letra en un indice para buscar en la matris algo tipo un map
int obtenerIndice(char letra)
{
	if (letra == 'I') return 0;
	if (letra == 'O') return 1;
	if (letra == 'T') return 2;
	if (letra == 'S') return 3;
	if (letra == 'Z') return 4;
	if (letra == 'J') return 5;
	if (letra == 'L') return 6;
	return 0;
}

Pieza crearPieza(char tipo)
{
	// Todas las piezas nacen en la misma zona de aparicion; su forma real
	// depende del tipo y de la rotacion almacenada en la estructura.
	Pieza nueva;
	nueva.tipo = tipo;
	nueva.x = 3;
	nueva.y = 0;
	nueva.rotacion = 0;

	return nueva;
}

int obtenerXBloque(const Pieza &pieza, int bloque)
{
	int indice = obtenerIndice(pieza.tipo);
	// FORMAS contiene desplazamientos relativos. Se suman a x e y para
	// obtener la coordenada absoluta de cada bloque en el tablero.
	return pieza.x + FORMAS[indice][pieza.rotacion][bloque].x;
}

int obtenerYBloque(const Pieza &pieza, int bloque)
{
	int indice = obtenerIndice(pieza.tipo);
	return pieza.y + FORMAS[indice][pieza.rotacion][bloque].y;
}

Color obtenerColorPieza(char tipo)
{
	switch (tipo)
	{
	case 'I': return SKYBLUE;
	case 'O': return YELLOW;
	case 'T': return PURPLE;
	case 'S': return GREEN;
	case 'Z': return RED;
	case 'J': return BLUE;
	case 'L': return ORANGE;
	}
	return WHITE;
}

void dibujarPieza(const Pieza &pieza, int xTablero, int yTablero, int tamCelda)
{
	Color color = obtenerColorPieza(pieza.tipo);

	for (int bloque = 0; bloque < 4; bloque++)
	{
		int columna = obtenerXBloque(pieza, bloque);
		int fila = obtenerYBloque(pieza, bloque);

		int x = xTablero + columna * tamCelda;
		int y = yTablero + fila * tamCelda;

		DrawRectangle(x + 2, y + 2, tamCelda - 4, tamCelda - 4, color);
	}
}

bool posicionValida(const Pieza &pieza, const Tablero &tablero)
{
	// Una pieza es valida solo si sus cuatro bloques estan dentro del tablero
	// y no ocupan una celda ya utilizada.
	for (int bloque = 0; bloque < 4; bloque++)
	{
		int columna = obtenerXBloque(pieza, bloque);
		int fila = obtenerYBloque(pieza, bloque);

		// Evita que se salga del mapa
		if (columna < 0 || columna >= 10){
			return false;
		}
		if (fila >= 20){
			return false;
		}
		if (fila < 0){
			return false;
		}
		if (tablero.obtenerCelda(fila, columna) != 0){
			return false; // para detectar choque
		}
	}

	return true;
}

bool moverPieza(Pieza &pieza, int movimientoX, int movimientoY, const Tablero &tablero)
{
	// Se prueba una copia antes de modificar la pieza real; si falla el choque,
	// la pieza conserva exactamente su posicion anterior.
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

bool rotarPieza(Pieza &pieza, const Tablero &tablero)
{
	// La rotacion tambien se valida como movimiento tentativo para no atravesar
	// paredes, piso u otras piezas.
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
