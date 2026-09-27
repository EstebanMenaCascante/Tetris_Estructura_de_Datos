#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ListaReplay.h"
#include "raylib.h"

class Juego
{
private:
	Tablero tablero;
	ColaPiezas cola;
	PilaHold hold;
	ListaReplay historial;
	Pieza piezaActual;

	int pantalla;
	float tiempoCaida;
	float velocidadCaida;
	float tiempoMovLateral;
	float retardoMovimiento;
	float tiempoReplay;
	float retardoReplay;

	NodoReplay *nodoPelicula;
	float tiempoPelicula;
	float velocidadPelicula;
	bool peliculaPausada;

	void cargarFotogramaPelicula();

	void actualizarInicio();
	void actualizarJugando(float deltaTime);
	void actualizarPausa();
	void actualizarGameOver();
	void actualizarPelicula(float deltaTime);

	void dibujarInicio();
	void dibujarJugando();
	void dibujarGameOver();
	void dibujarPelicula();

public:
	Juego();
	void actualizar(float deltaTime);
	void dibujar();
};

#endif
