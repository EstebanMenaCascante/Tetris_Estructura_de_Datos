#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

class Jugador
{
private:
    std::string nombre;
    int puntaje;
    int ultimoPuntajeAnadido;
    int lineasTotales;
    int maxLineasCombo;
    float tiempoPartida;

public:
    Jugador();
    Jugador(std::string nombreInicial);

    std::string getNombre() const;
    void setNombre(std::string nuevoNombre);

    int getPuntaje() const;
    int getUltimoPuntaje() const;
    void sumarPuntaje(int puntos);

    int getLineasTotales() const;
    void sumarLineas(int lineas);

    int getMaxLineasCombo() const;
    void actualizarMaxCombo(int combo);

    float getTiempoPartida() const;
    void sumarTiempo(float deltaTime);

    void reiniciarEstadisticas();
};

#endif
