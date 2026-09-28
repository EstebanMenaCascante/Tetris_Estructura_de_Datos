#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H

struct Evento {
    int tipo; // 1: Invertidos, 2: Espejo, 3: Bomba, 4: Comodin
    float tiempoActivacion;
};

struct NodoEvento {
    Evento evento;
    NodoEvento* siguiente;
};

class ColaEventos {
private:
    NodoEvento* frente;

public:
    ColaEventos();
    ~ColaEventos();

    void encolar(Evento e);
    Evento desencolar();
    Evento verFrente() const;
    bool estaVacia() const;
    void vaciar();
};

#endif
