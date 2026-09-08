#include <iostream>
#include "Tablero.h"
#include "Pieza.h"
#include "raylib.h"
#include <cmath>
using namespace std;

int main(int argc, char *argv[])
{
	const int ANCHO = 900;
	const int ALTO = 700;

	InitWindow(ANCHO, ALTO, "Tetris - Estructuras de Datos");

	SetTargetFPS(60);

	//--------------------------------------------------
	// Crear tablero
	//--------------------------------------------------

	Tablero tablero;

	// Crear primera pieza
	Pieza piezaActual = crearPieza(T);

	//--------------------------------------------------
	// Ciclo principal
	//--------------------------------------------------

	while (!WindowShouldClose())
	{
		// --------------------------------------------------
		// Controles
		// --------------------------------------------------

		if (IsKeyPressed(KEY_LEFT))
		{
			moverPieza(
				piezaActual,
				-1,
				0,
				tablero);
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			moverPieza(
				piezaActual,
				1,
				0,
				tablero);
		}

		if (IsKeyPressed(KEY_DOWN))
		{
			moverPieza(
				piezaActual,
				0,
				1,
				tablero);
		}

		if (IsKeyPressed(KEY_UP))
		{
			rotarPieza(
				piezaActual,
				tablero);
		}

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

		tablero.dibujar(320, 60, 28); // X, Y, Tamano de cada celda

		dibujarPieza(piezaActual, 320, 60, 28);

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
