#include <iostream>
#include "Tablero.h"
#include "Pieza.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "raylib.h"
#include <cmath>
using namespace std;

int main(int argc, char *argv[])
{
	const int ANCHO = 900;
	const int ALTO = 700;

	InitWindow(ANCHO, ALTO, "Tetris - Estructuras de Datos");

	SetTargetFPS(60);

	Tablero tablero;

	ColaPiezas cola;
	PilaHold hold;

	Pieza piezaActual = crearPieza(cola.desencolar());
	cola.rellenarSiEsNecesario();

	float tiempoCaida = 0.0f;
	float velocidadCaida = 0.5f;
	
	float tiempoMovLateral = 0.0f;
	float retardoMovimiento = 0.12f;

	while (!WindowShouldClose())
	{

		float deltaTime = GetFrameTime();
		tiempoCaida += deltaTime;
		if (tiempoCaida >= velocidadCaida)
		{
			tiempoCaida = 0.0f;

			if (!moverPieza(piezaActual, 0, 1, tablero))
			{
				for (int bloque = 0; bloque < 4; bloque++)
				{
					int col = obtenerXBloque(piezaActual, bloque);
					int fila = obtenerYBloque(piezaActual, bloque);

					tablero.colocarCelda(fila, col, (int)piezaActual.tipo + 1);
				}
				tablero.limpiarFilas();
				piezaActual = crearPieza(cola.desencolar());
				cola.rellenarSiEsNecesario();

				hold.desbloquear();
			}
		}

		if (IsKeyPressed(KEY_LEFT)) {
			moverPieza(piezaActual, -1, 0, tablero);
			tiempoMovLateral = 0.0f;
		} 
		else if (IsKeyDown(KEY_LEFT)) {
			tiempoMovLateral += deltaTime;
			if (tiempoMovLateral >= retardoMovimiento) {
				moverPieza(piezaActual, -1, 0, tablero);
				tiempoMovLateral = 0.0f;
			}
		}
		if (IsKeyPressed(KEY_RIGHT)) {
			moverPieza(piezaActual, 1, 0, tablero);
			tiempoMovLateral = 0.0f;
		}
		else if (IsKeyDown(KEY_RIGHT)) {
			tiempoMovLateral += deltaTime;
			if (tiempoMovLateral >= retardoMovimiento) {
				moverPieza(piezaActual, 1, 0, tablero);
				tiempoMovLateral = 0.0f;
			}
		}
		if (IsKeyDown(KEY_DOWN)) {
			velocidadCaida = 0.05f;
		} else {
			velocidadCaida = 0.5f;
		}

		if (IsKeyPressed(KEY_UP))
		{
			rotarPieza(
				piezaActual,
				tablero);
		}
		
		if (IsKeyPressed(KEY_C) && hold.puedeIntercambiar())
		{
			if (hold.estaVacia())
			{
				hold.apilar(piezaActual.tipo);
				piezaActual = crearPieza(cola.desencolar());
				cola.rellenarSiEsNecesario();
			}
			else
			{
				TipoPieza guardada = hold.desapilar();
				hold.apilar(piezaActual.tipo);
				piezaActual = crearPieza(guardada);
			}
			hold.bloquear();
		}

		BeginDrawing();

		ClearBackground(Color{18, 20, 28, 255});

		DrawText("TETRIS", 60, 60, 50, RAYWHITE);

		DrawText("Estructuras de Datos", 30, 120, 24, GRAY);

		tablero.dibujar(320, 60, 28);

		dibujarPieza(piezaActual, 320, 60, 28);

		DrawText("TABLERO", 650, 100, 28, RAYWHITE);

		DrawText("10 columnas", 650, 150, 20, LIGHTGRAY);

		DrawText("20 filas", 650, 180, 20, LIGHTGRAY);

		DrawText("Lista enlazada", 650, 230, 20, GREEN);
		
		DrawText("HOLD (Tecla C)", 50, 200, 20, RAYWHITE);
		
		if (!hold.estaVacia())
		{
			Pieza pHold = crearPieza(hold.verPieza());
			pHold.x = 0;
			pHold.y = 0;
			dibujarPieza(pHold, 50, 240, 28);
		}
		
		DrawText("SIGUIENTES", 650, 300, 20, RAYWHITE);
		
		for (int i = 0; i < 3; i++)
		{
			Pieza pSiguiente = crearPieza(cola.verSiguiente(i));
			pSiguiente.x = 0; 
			pSiguiente.y = 0;
			dibujarPieza(pSiguiente, 650, 340 + (i * 90), 28); 
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
