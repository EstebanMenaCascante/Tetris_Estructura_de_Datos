#include "Jugador.h"

Jugador::Jugador()
{
    nombre = "Invitado";
    reiniciarEstadisticas();
}

Jugador::Jugador(std::string nombreInicial)
{
    nombre = nombreInicial;
    reiniciarEstadisticas();
}

void Jugador::reiniciarEstadisticas()
{
    puntaje = 0;
    ultimoPuntajeAnadido = 0;
    lineasTotales = 0;
    maxLineasCombo = 0;
    tiempoPartida = 0.0f;
}

std::string Jugador::getNombre() const { return nombre; }
void Jugador::setNombre(std::string nuevoNombre)
{
    nombre = nuevoNombre;
}

int Jugador::getPuntaje() const { return puntaje; }
int Jugador::getUltimoPuntaje() const
{
    return ultimoPuntajeAnadido;
}

void Jugador::sumarPuntaje(int puntos)
{
    puntaje += puntos;
    ultimoPuntajeAnadido = puntos;
}

int Jugador::getLineasTotales() const
{
    return lineasTotales;
}
void Jugador::sumarLineas(int lineas)
{
    lineasTotales += lineas;
}

int Jugador::getMaxLineasCombo() const
{
    return maxLineasCombo;
}
void Jugador::actualizarMaxCombo(int combo)
{
    if (combo > maxLineasCombo)
    {
        maxLineasCombo = combo;
    }
}

float Jugador::getTiempoPartida() const
{
    return tiempoPartida;
}
void Jugador::sumarTiempo(float deltaTime)
{
    tiempoPartida += deltaTime;
}
