#ifndef TABLERO_H
#define TABLERO_H

class Tablero {
private: 
	struct NodoFila{
		int celdas[10];
		NodoFila* siguiente;

		NodoFila();
	};

	NodoFila* cabeza;

	static const int FILAS = 20;
	static const int COLUMNAS = 10;

	NodoFila* obtenerFila(int numeroFila) const;

	public:
		Tablero();
		~Tablero();

		void reiniciar();
		void colocarCelda(int fila, int columna, int valor);
		int obtenerCelda(int fila, int columna) const;
		void dibujar(int xInicial, int yInicial, int tamCelda) const;
};
#endif 