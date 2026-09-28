#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ListaReplay.h"
#include "Jugador.h"
#include "Archivo.h"
#include "ColaEventos.h"
#include "raylib.h"
#include <string>
#include <vector>

class Juego
{
private:
    Tablero tablero;
    ColaPiezas cola;
    PilaHold hold;
    ListaReplay historial;
    Pieza piezaActual;
    Jugador jugadorActual;

    Archivo gestorArchivos;
    int metodoOrdenamiento;
    std::vector<RegistroPuntaje> top10;

    ColaEventos eventos;
    int ultimoTipoEvento;
    float temporizadorAlerta;
    std::string textoAlerta;

    bool controlesInvertidos;
    float finControlesInvertidos;

    bool piezaEspejoActiva;
    Pieza piezaEspejo;
    bool espejoBloqueado;
    bool principalBloqueado;

    bool bombaActiva;
    char piezaComodinReservada;

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

    std::string nombreTemp;
    int framesCursor;

public:
    Juego();

    void cargarFotogramaPelicula();

    void programarSiguienteEvento();
    void ejecutarEvento(Evento e);

    void actualizarInicio();
    void actualizarEscribirNombre();
    void actualizarJugando(float deltaTime);
    void actualizarPausa();
    void actualizarGameOver();
    void actualizarPelicula(float deltaTime);
    void actualizarEstadisticas();
    void actualizarComodin();

    void dibujarInicio();
    void dibujarEscribirNombre();
    void dibujarJugando();
    void dibujarPausa();
    void dibujarGameOver();
    void dibujarPelicula();
    void dibujarEstadisticas();
    void dibujarComodin();
    void actualizar(float deltaTime);
    void dibujar();
};

#endif
