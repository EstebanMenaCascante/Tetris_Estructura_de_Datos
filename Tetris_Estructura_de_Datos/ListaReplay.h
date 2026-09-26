#ifndef LISTAREPLAY_H
#define LISTAREPLAY_H
#include "Pieza.h"
#include "Tablero.h"
#include "PilaHold.h"

struct EstadoJuego {
	Pieza piezaActual;
	int tableroRepleay[20][10];
	
	char piezaHold;
	bool holdVacio;
	bool holdBloqueado;
};

struct NodoReplay {
	EstadoJuego estado;
	NodoReplay* siguiente;
	NodoReplay* anterior;
};

class ListaReplay {
private:
	NodoReplay* primero;
	NodoReplay* actual; // Puntero para saber en qué momento se está (para deshacer/rehacer)
	
public:
	ListaReplay();
	~ListaReplay();
	
	void registrarEstado(const Pieza& p, const Tablero& t, const PilaHold& h);
	void eliminarFuturo(); // Función auxiliar para deahacer y luego rehacer
	bool puedeDeshacer() const;
	bool puedeRehacer() const;
	
	EstadoJuego deshacer();
	EstadoJuego rehacer();
	
	NodoReplay* obtenerPrimero() const;
};

#endif
