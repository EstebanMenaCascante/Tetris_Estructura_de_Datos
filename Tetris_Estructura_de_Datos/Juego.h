#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ListaReplay.h"
#include "Jugador.h"
#include "raylib.h"
#include <string>

class Juego {
private:
    Tablero tablero;
    ColaPiezas cola;
    PilaHold hold;
    ListaReplay historial;
    Pieza piezaActual;
    Jugador jugadorActual;

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

    std::string nombreTemp; //almacena temporalmente el nombre del jugador mientras lo escribe
    int framesCursor;

    void cargarFotogramaPelicula();

    void actualizarInicio();
    void actualizarEscribirNombre();
    void actualizarJugando(float deltaTime);
    void actualizarPausa();
    void actualizarGameOver();
    void actualizarPelicula(float deltaTime);

    void dibujarInicio();
    void dibujarEscribirNombre(); 
    void dibujarJugando();
    void dibujarPausa();
    void dibujarGameOver();
    void dibujarPelicula();

public:
    Juego();
    void actualizar(float deltaTime);
    void dibujar();
};

#endif
