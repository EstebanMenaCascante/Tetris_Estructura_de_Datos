#include "Tablero.h"
#include "raylib.h"

Tablero::NodoFila::NodoFila()
{
	siguiente = nullptr;
	
	for (int i = 0; i < 10; i++)
	{
		celdas[i] = 0;
	}
}


Tablero::Tablero()
{
	primero = nullptr;
	
	NodoFila* ultimo = nullptr;
	
	// Crear exactamente 20 filas
	for (int fila = 0; fila < FILAS; fila++)
	{
		NodoFila* nuevaFila = new NodoFila();
		
		if (primero == nullptr)
		{
			primero = nuevaFila;
		}
		else
		{
			ultimo->siguiente = nuevaFila;
		}
		
		ultimo = nuevaFila;
	}
}


//------------------------------------------------------
// Destructor
//------------------------------------------------------
Tablero::~Tablero()
{
	NodoFila* actual = primero;
	
	while (actual != nullptr)
	{
		NodoFila* eliminar = actual;
		
		actual = actual->siguiente;
		
		delete eliminar;
	}
	
	primero = nullptr;
}


//------------------------------------------------------
// Obtener una fila especifica
//------------------------------------------------------
Tablero::NodoFila* Tablero::obtenerFila(int numeroFila) const
{
	if (numeroFila < 0 || numeroFila >= FILAS)
	{
		return nullptr;
	}
	
	NodoFila* actual = primero;
	
	int contador = 0;
	
	while (actual != nullptr && contador < numeroFila)
	{
		actual = actual->siguiente;
		contador++;
	}
	
	return actual;
}


//------------------------------------------------------
// Vaciar el tablero
//------------------------------------------------------
void Tablero::reiniciar()
{
	NodoFila* actual = primero;
	
	while (actual != nullptr)
	{
		for (int columna = 0; columna < COLUMNAS; columna++)
		{
			actual->celdas[columna] = 0;
		}
		
		actual = actual->siguiente;
	}
}


//------------------------------------------------------
// Colocar un valor en una celda
//------------------------------------------------------
void Tablero::colocarCelda(int fila, int columna, int valor)
{
	if (columna < 0 || columna >= COLUMNAS)
	{
		return;
	}
	
	NodoFila* nodoFila = obtenerFila(fila);
	
	if (nodoFila != nullptr)
	{
		nodoFila->celdas[columna] = valor;
	}
}


//------------------------------------------------------
// Consultar una celda
//------------------------------------------------------
int Tablero::obtenerCelda(int fila, int columna) const
{
	if (columna < 0 || columna >= COLUMNAS)
	{
		return -1;
	}
	
	NodoFila* nodoFila = obtenerFila(fila);
	
	if (nodoFila == nullptr)
	{
		return -1;
	}
	
	return nodoFila->celdas[columna];
}

bool Tablero::filaCompleta(int fila) const
{
	NodoFila* nodoFila = obtenerFila(fila);
	
	if (nodoFila == nullptr)
	{
		return false;
	}
	
	for (int columna = 0; columna < COLUMNAS; columna++)
	{
		if (nodoFila->celdas[columna] == 0)
		{
			return false;
		}
	}
	
	return true;
}

void Tablero::eliminarFila(int fila)
{
	if (fila < 0 || fila >= FILAS || primero == nullptr)
	{
		return;
	}
	
	// Si queremos eliminar la primera fila
	if (fila == 0)
	{
		NodoFila* eliminar = primero;
		
		primero = primero->siguiente;
		
		delete eliminar;
		
		return;
	}
	
	NodoFila* anterior = primero;
	
	// Llegamos al nodo anterior al que queremos eliminar
	for (int i = 0; i < fila - 1; i++)
	{
		anterior = anterior->siguiente;
	}
	
	NodoFila* eliminar = anterior->siguiente;
	
	anterior->siguiente = eliminar->siguiente;
	
	delete eliminar;
}

void Tablero::insertarFilaVaciaInicio()
{
	NodoFila* nuevaFila = new NodoFila();
	
	nuevaFila->siguiente = primero;
	
	primero = nuevaFila;
}

int Tablero::limpiarFilas()
{
	int eliminadas = 0;
	
	int fila = FILAS - 1;
	
	while (fila >= 0)
	{
		if (filaCompleta(fila))
		{
			eliminarFila(fila);
			
			insertarFilaVaciaInicio();
			
			eliminadas++;
			
			// No bajamos de fila todavía.
			// La fila superior acaba de caer a esta posición.
		}
		else
		{
			fila--;
		}
	}
	
	return eliminadas;
}

//------------------------------------------------------
// Dibujar el tablero usando raylib
//------------------------------------------------------
void Tablero::dibujar(int xInicial, int yInicial, int tamCelda) const
{
	NodoFila* actual = primero;
	
	int fila = 0;
	
	while (actual != nullptr)
	{
		for (int columna = 0; columna < COLUMNAS; columna++)
		{
			int x = xInicial + columna * tamCelda;
			int y = yInicial + fila * tamCelda;
			
			int valor = actual->celdas[columna];
			
			//--------------------------------------------------
			// Celda vacia
			//--------------------------------------------------
			
			if (valor == 0)
			{
				DrawRectangle(x + 1, y + 1, tamCelda - 2, tamCelda - 2, Color{30, 32, 40, 255});
			}
			
			//--------------------------------------------------
			// Celda ocupada
			//--------------------------------------------------
			
			else
			{
				Color colorBloque = WHITE;
				
				switch (valor)
				{
				case 1:
					colorBloque = SKYBLUE;
					break;
					
				case 2:
					colorBloque = YELLOW;
					break;
					
				case 3:
					colorBloque = PURPLE;
					break;
					
				case 4:
					colorBloque = GREEN;
					break;
					
				case 5:
					colorBloque = RED;
					break;
					
				case 6:
					colorBloque = BLUE;
					break;
					
				case 7:
					colorBloque = ORANGE;
					break;
				}
				
				DrawRectangle(x + 2,y + 2, tamCelda - 4, tamCelda - 4, colorBloque);
			}
			
			//--------------------------------------------------
			// Linea de la cuadricula
			//--------------------------------------------------
			
			DrawRectangleLines(x, y, tamCelda, tamCelda, Color{65, 68, 78, 255});
		}
		
		actual = actual->siguiente;
		
		fila++;
	}
	
	//------------------------------------------------------
	// Borde exterior
	//------------------------------------------------------
	
	Rectangle borde = {(float)xInicial, (float)yInicial, (float)(COLUMNAS * tamCelda), (float)(FILAS * tamCelda)};
	
	DrawRectangleLinesEx(borde, 3, RAYWHITE);
}
