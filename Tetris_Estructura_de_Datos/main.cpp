#include<iostream>
#include "Tablero.h"
#include "raylib.h"
#include <cmath>
using namespace std;

int main (int argc, char *argv[]) {
	const int ANCHO = 900;
	const int ALTO = 700;
	
	InitWindow(ANCHO, ALTO, "Tetris - Estructuras de Datos");
	
	SetTargetFPS(60);
	
	//--------------------------------------------------
	// Crear tablero
	//--------------------------------------------------
	
	Tablero tablero;
	
	
	//--------------------------------------------------
	// Bloques temporales solamente para probar
	// que la lista y el dibujo funcionan.
	//--------------------------------------------------
	
	tablero.colocarCelda(19, 2, 1);
	tablero.colocarCelda(19, 3, 1);
	tablero.colocarCelda(19, 4, 1);
	tablero.colocarCelda(19, 5, 1);
	
	tablero.colocarCelda(18, 4, 3);
	tablero.colocarCelda(18, 5, 3);
	
	tablero.colocarCelda(17, 5, 4);
	
	tablero.colocarCelda(16, 5, 5);
	
	
	//--------------------------------------------------
	// Ciclo principal
	//--------------------------------------------------
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ClearBackground(Color{18, 20, 28, 255});
		
		
		//--------------------------------------------------
		// Titulo
		//--------------------------------------------------
		
		DrawText("TETRIS", 60, 60, 50, RAYWHITE);
		
		DrawText("Estructuras de Datos", 30, 120, 24, GRAY);
		
		
		//--------------------------------------------------
		// Tablero
		//--------------------------------------------------
		
		tablero.dibujar(320, 60, 28);// X, Y, Tamano de cada celda
						
		
		
		//--------------------------------------------------
		// Informacion temporal
		//--------------------------------------------------
		
		DrawText("TABLERO", 650, 100, 28, RAYWHITE);
		
		DrawText("10 columnas", 650, 150, 20, LIGHTGRAY);
		
		DrawText("20 filas", 650, 180, 20, LIGHTGRAY);
		
		DrawText("Lista enlazada", 650, 230, 20, GREEN);
		
		
		EndDrawing();
	}
	
	
	CloseWindow();
	
	return 0;
}

