#include <iostream>
#include "Tablero.h"
#include "Pieza.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ListaReplay.h"
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
	
	ListaReplay historial;
	historial.registrarEstado(piezaActual, tablero, hold);	

	float tiempoCaida = 0.0f;
	float velocidadCaida = 0.5f;

	float tiempoMovLateral = 0.0f;
	float retardoMovimiento = 0.12f;
	
	float tiempoReplay = 0.0f;
	float retardoReplay = 0.08f;

	bool gameOver = false;

	while (!WindowShouldClose())
	{

		float deltaTime = GetFrameTime();
		if (!gameOver)
		{
			bool hizoMovimiento = false;
			
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
						tablero.colocarCelda(fila, col, obtenerIndice(piezaActual.tipo) + 1); // Guardar numero en tablero
					}
					tablero.limpiarFilas();
					piezaActual = crearPieza(cola.desencolar());
					cola.rellenarSiEsNecesario();

					if (!posicionValida(piezaActual, tablero))
					{
						gameOver = true;
					}
					hold.desbloquear();
					hizoMovimiento = true;
				}
				else{
					hizoMovimiento = true;
				}
			}
			
			

			if (IsKeyPressed(KEY_LEFT))
			{
				moverPieza(piezaActual, -1, 0, tablero);
				tiempoMovLateral = 0.0f;
				hizoMovimiento = true;
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				tiempoMovLateral += deltaTime;
				if (tiempoMovLateral >= retardoMovimiento)
				{
					moverPieza(piezaActual, -1, 0, tablero);
					tiempoMovLateral = 0.0f;
					hizoMovimiento = true;
				}
			}
			if (IsKeyPressed(KEY_RIGHT))
			{
				moverPieza(piezaActual, 1, 0, tablero);
				tiempoMovLateral = 0.0f;
				hizoMovimiento = true;
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				tiempoMovLateral += deltaTime;
				if (tiempoMovLateral >= retardoMovimiento)
				{
					moverPieza(piezaActual, 1, 0, tablero);
					tiempoMovLateral = 0.0f;
					hizoMovimiento = true;
				}
			}
			if (IsKeyDown(KEY_DOWN))
			{
				velocidadCaida = 0.05f;
				hizoMovimiento = true;
			}
			else
			{
				velocidadCaida = 0.5f;
			}

			if (IsKeyPressed(KEY_UP))
			{
				rotarPieza(piezaActual, tablero);
				hizoMovimiento = true;
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
					char guardada = hold.desapilar(); // Para saber la pieza guardada
					hold.apilar(piezaActual.tipo);
					piezaActual = crearPieza(guardada);
				}
				hold.bloquear();
			}
			bool intentarDeshacer = false;
			bool intentarRehacer = false;
			
			if (IsKeyPressed(KEY_Z)) {
				intentarDeshacer = true;
				tiempoReplay = 0.0f;
			} else if (IsKeyDown(KEY_Z)) {
				tiempoReplay += deltaTime;
				if (tiempoReplay >= retardoReplay) {
					intentarDeshacer = true;
					tiempoReplay = 0.0f;
				}
			}
			
			if (IsKeyPressed(KEY_X)) {
				intentarRehacer = true;
				tiempoReplay = 0.0f;
			} else if (IsKeyDown(KEY_X)) {
				tiempoReplay += deltaTime;
				if (tiempoReplay >= retardoReplay) {
					intentarRehacer = true;
					tiempoReplay = 0.0f;
				}
			}
			
			if (intentarDeshacer && historial.puedeDeshacer()) {
				EstadoJuego deshacer = historial.deshacer();
				
				piezaActual = deshacer.piezaActual;
				for (int fila = 0; fila < 20; fila++) {
					for (int col = 0; col < 10; col++) {
						tablero.colocarCelda(fila, col, deshacer.tableroRepleay[fila][col]);
					}
				}
				if (!hold.estaVacia()) hold.desapilar();
				if (!deshacer.holdVacio) hold.apilar(deshacer.piezaHold);
				if (deshacer.holdBloqueado) hold.bloquear(); else hold.desbloquear();
				
				tiempoCaida = 0.0f;
			}
			if (intentarRehacer && historial.puedeRehacer()) {
				EstadoJuego rehacer = historial.rehacer();
				
				piezaActual = rehacer.piezaActual;
				for (int fila = 0; fila < 20; fila++) {
					for (int col = 0; col < 10; col++) {
						tablero.colocarCelda(fila, col, rehacer.tableroRepleay[fila][col]);
					}
				}
				if (!hold.estaVacia()) hold.desapilar();
				if (!rehacer.holdVacio) hold.apilar(rehacer.piezaHold);
				if (rehacer.holdBloqueado) hold.bloquear(); else hold.desbloquear();
				
				tiempoCaida = 0.0f;
			}
			if (hizoMovimiento) {
				historial.registrarEstado(piezaActual, tablero, hold);
			}
		}

		if (!gameOver)
		{
			BeginDrawing();

			ClearBackground(Color{18, 20, 28, 255});

			DrawText("TETRIS", 60, 60, 50, RAYWHITE);

			DrawText("Estructuras de Datos", 30, 120, 24, GRAY);

			tablero.dibujar(320, 60, 28);

			dibujarPieza(piezaActual, 320, 60, 28);

			DrawText("TABLERO", 650, 100, 28, RAYWHITE);
			
			DrawText("Cambio (Tecla C)", 50, 200, 20, RAYWHITE);

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
		}
		else
		{
			DrawText("GAME OVER", 250, 250, 60, RED);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
