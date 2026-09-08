#define TABLERO_H

class Tablero
{
private:
	
	struct NodoFila
	{
		int celdas[10];
		NodoFila* siguiente;
		
		NodoFila();
	};
	
	NodoFila* primero;
	
	static const int FILAS = 20;
	static const int COLUMNAS = 10;
	
	NodoFila* obtenerFila(int numeroFila) const;
	
	// Nuevos métodos internos
	bool filaCompleta(int fila) const;
	void eliminarFila(int fila);
	void insertarFilaVaciaInicio();
	
public:
	
	Tablero();
	~Tablero();
	
	void reiniciar();
	
	void colocarCelda(int fila, int columna, int valor);
	
	int obtenerCelda(int fila, int columna) const;
	
	// Devuelve cuántas filas eliminó
	int limpiarFilas();
	
	void dibujar(int xInicial,
				 int yInicial,
				 int tamCelda) const;
};

