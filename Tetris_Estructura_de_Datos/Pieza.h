#define PIEZA_H

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

// Crea una pieza en la posicion inicial
Pieza crearPieza(TipoPieza tipo);

// Devuelven la posicion de cada uno de los 4 bloques
int obtenerXBloque(const Pieza& pieza, int bloque);
int obtenerYBloque(const Pieza& pieza, int bloque);

// Dibuja la pieza usando raylib
void dibujarPieza(
				  const Pieza& pieza,
				  int xTablero,
				  int yTablero,
				  int tamCelda
				  );
