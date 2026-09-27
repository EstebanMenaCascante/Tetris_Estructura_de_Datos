#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ListaReplay.h"
#include "Jugador.h"
#include "Archivo.h"
#include "raylib.h"
#include <string>
#include <vector>

class Juego {
private:
    Tablero tablero;
    ColaPiezas cola;
    PilaHold hold;
    ListaReplay historial;
    Pieza piezaActual;
    Jugador jugadorActual;
    
    Archivo gestorArchivos;
    int metodoOrdenamiento; // 0 = Insertion Sort, 1 = Merge Sort
    std::vector<RegistroPuntaje> top10; // Para mostrar en la pantalla

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

    std::string nombreTemp; // Para guardar lo que el jugador teclea
    int framesCursor; // Para hacer parpadear el cursor

    void cargarFotogramaPelicula();

    void actualizarInicio();
    void actualizarEscribirNombre(); 
    void actualizarJugando(float deltaTime);
    void actualizarPausa();
    void actualizarGameOver();
    void actualizarPelicula(float deltaTime);
    void actualizarEstadisticas();

    void dibujarInicio();
    void dibujarEscribirNombre(); 
    void dibujarJugando();
    void dibujarPausa();
    void dibujarGameOver();
    void dibujarPelicula();
    void dibujarEstadisticas();

public:
    Juego();
    void actualizar(float deltaTime);
    void dibujar();
};

#endif
