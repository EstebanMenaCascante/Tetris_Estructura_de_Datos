#include "Juego.h"
#include <string>

Juego::Juego()
{
    pantalla = 0;
    tiempoCaida = 0.0f;
    velocidadCaida = 0.5f;
    tiempoMovLateral = 0.0f;
    retardoMovimiento = 0.12f;
    tiempoReplay = 0.0f;
    retardoReplay = 0.08f;

    velocidadPelicula = 0.15f;
    peliculaPausada = false;
    nodoPelicula = nullptr;
    tiempoPelicula = 0.0f;
    nombreTemp = "";
    framesCursor = 0;

    metodoOrdenamiento = 0;

    // Variables de eventos
    ultimoTipoEvento = 0;
    temporizadorAlerta = 0.0f;
    textoAlerta = "";
    controlesInvertidos = false;
    finControlesInvertidos = 0.0f;
    piezaEspejoActiva = false;
    espejoBloqueado = false;
    principalBloqueado = false;
    bombaActiva = false;
    piezaComodinReservada = ' ';

    piezaActual = crearPieza(cola.desencolar());
    cola.rellenarSiEsNecesario();
    historial.registrarEstado(piezaActual, tablero, hold);
}

void Juego::programarSiguienteEvento()
{
    float proxTiempo = jugadorActual.getTiempoPartida() + GetRandomValue(35, 60);
    int proxTipo;
    do
    {
        proxTipo = GetRandomValue(1, 4);
    } while (proxTipo == ultimoTipoEvento); // Para evitar el mismo evento 2 veces seguidas

    ultimoTipoEvento = proxTipo;

    Evento e;
    e.tipo = proxTipo;
    e.tiempoActivacion = proxTiempo;
    eventos.encolar(e); // Insertar ordenado
}

void Juego::ejecutarEvento(Evento e)
{
    if (e.tipo == 1)
    { // 1 = Controles Invertidos
        textoAlerta = "EVENTO: CONTROLES INVERTIDOS";
        temporizadorAlerta = 3.0f;
        controlesInvertidos = true;
        finControlesInvertidos = jugadorActual.getTiempoPartida() + 15.0f; // Dura 15 segundos
    }
    else if (e.tipo == 2)
    { // 2 = Pieza Espejo
        textoAlerta = "EVENTO: PIEZA ESPEJO";
        temporizadorAlerta = 3.0f;
        piezaEspejoActiva = true;
        espejoBloqueado = false;
        principalBloqueado = false;

        piezaEspejo = crearPieza(piezaActual.tipo);
        piezaEspejo.x = 9 - piezaActual.x; // Aparece al lado opuesto
        piezaEspejo.y = piezaActual.y;
    }
    else if (e.tipo == 3)
    { // 3 = Pieza Bomba
        textoAlerta = "EVENTO: PIEZA BOMBA";
        temporizadorAlerta = 3.0f;
        bombaActiva = true; // La pieza actual se convierte en bomba
    }
    else if (e.tipo == 4)
    { // 4 = Pieza Comodín
        textoAlerta = "EVENTO: PIEZA COMODIN";
        temporizadorAlerta = 3.0f;
        pantalla = 7; // Ir a pantalla de selección
    }

    // Siempre programar uno nuevo tras ejecutar para mantener el ciclo
    programarSiguienteEvento();
}

void Juego::actualizar(float deltaTime)
{
    if (pantalla == 0)
    {
        actualizarInicio();
    }
    else if (pantalla == 1)
    {
        actualizarJugando(deltaTime);
    }
    else if (pantalla == 2)
    {
        actualizarPausa();
    }
    else if (pantalla == 3)
    {
        actualizarGameOver();
    }
    else if (pantalla == 4)
    {
        actualizarPelicula(deltaTime);
    }
    else if (pantalla == 5)
    {
        actualizarEscribirNombre();
    }
    else if (pantalla == 6)
    {
        actualizarEstadisticas();
    }
    else if (pantalla == 7)
    {
        actualizarComodin();
    }
}

void Juego::dibujar()
{
    if (pantalla == 0)
    {
        dibujarInicio();
    }
    else if (pantalla == 1)
    {
        dibujarJugando();
    }
    else if (pantalla == 2)
    {
        dibujarPausa();
    }
    else if (pantalla == 3)
    {
        dibujarGameOver();
    }
    else if (pantalla == 4)
    {
        dibujarPelicula();
    }
    else if (pantalla == 5)
    {
        dibujarEscribirNombre();
    }
    else if (pantalla == 6)
    {
        dibujarEstadisticas();
    }
    else if (pantalla == 7)
    {
        dibujarComodin();
    }
}

void Juego::actualizarInicio()
{
    Vector2 raton = GetMousePosition();
    Rectangle btnJugar = {350, 300, 200, 50};
    Rectangle btnJugador = {350, 380, 200, 50};
    Rectangle btnStats = {350, 460, 200, 50};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(raton, btnJugar))
        {
            jugadorActual.reiniciarEstadisticas();
            pantalla = 1;

            // Reiniciar eventos
            eventos.vaciar();
            ultimoTipoEvento = 0;
            controlesInvertidos = false;
            piezaEspejoActiva = false;
            principalBloqueado = false;
            espejoBloqueado = false;
            bombaActiva = false;
            piezaComodinReservada = ' ';
            temporizadorAlerta = 0.0f;
            textoAlerta = "";

            // Programar el primer evento de la partida (habrá un descanso inicial natural)
            programarSiguienteEvento();
        }
        else if (CheckCollisionPointRec(raton, btnJugador))
        {
            nombreTemp = "";
            pantalla = 5;
        }
        else if (CheckCollisionPointRec(raton, btnStats))
        {
            pantalla = 6;
            top10 = gestorArchivos.cargarPuntajes();
        }
    }
}

void Juego::actualizarEscribirNombre()
{
    int tecla = GetCharPressed();

    while (tecla > 0)
    {
        if ((tecla >= 32) && (tecla <= 125) && (nombreTemp.length() < 15))
        {
            nombreTemp += (char)tecla;
        }
        tecla = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (nombreTemp.length() > 0)
        {
            nombreTemp.pop_back();
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        if (nombreTemp.length() > 0)
        {
            jugadorActual.setNombre(nombreTemp);
        }
        pantalla = 0;
    }
}

void Juego::dibujarEscribirNombre()
{
    DrawText("INGRESA TU NOMBRE", 250, 200, 40, RAYWHITE);
    DrawRectangle(250, 280, 400, 60, LIGHTGRAY);
    DrawText(nombreTemp.c_str(), 270, 295, 30, BLACK);

    framesCursor++;
    if (((framesCursor / 30) % 2) == 0 && nombreTemp.length() < 15)
    {
        DrawText("_", 270 + MeasureText(nombreTemp.c_str(), 30), 295, 30, BLACK);
    }
    DrawText("Presiona ENTER para guardar", 270, 400, 20, GRAY);
}

void Juego::actualizarEstadisticas()
{
    Vector2 raton = GetMousePosition();
    Rectangle btnVolver = {350, 600, 200, 50};
    Rectangle btnMetodo = {300, 100, 300, 40};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(raton, btnVolver))
        {
            pantalla = 0;
        }
        else if (CheckCollisionPointRec(raton, btnMetodo))
        {
            metodoOrdenamiento = (metodoOrdenamiento == 0) ? 1 : 0;
            if (metodoOrdenamiento == 0)
            {
                gestorArchivos.insertionSort(top10);
            }
            else
            {
                gestorArchivos.mergeSort(top10, 0, (int)top10.size() - 1);
            }
        }
    }
}

void Juego::dibujarEstadisticas()
{
    DrawText("MEJORES PUNTAJES (TOP 10)", 250, 40, 30, GREEN);

    Rectangle btnMetodo = {300, 100, 300, 40};
    DrawRectangleRec(btnMetodo, DARKBLUE);

    const char *textoMetodo;

    if (metodoOrdenamiento == 0)
    {
        textoMetodo = "Orden: Insertion Sort (O(n^2))";
    }
    else
    {
        textoMetodo = "Orden: Merge Sort (O(n log n))";
    }

    DrawText(textoMetodo, 315, 110, 18, RAYWHITE);

    int y = 180;
    for (int i = 0; i < top10.size(); i++)
    {
        DrawText(TextFormat("%d. %s", i + 1, top10[i].nombre.c_str()), 300, y, 25, RAYWHITE);
        DrawText(TextFormat("%d", top10[i].puntaje), 550, y, 25, YELLOW);
        y += 40;
    }

    Rectangle btnVolver = {350, 600, 200, 50};
    DrawRectangleRec(btnVolver, RED);
    DrawText("Volver", 415, 615, 20, WHITE);
}

void Juego::actualizarPausa()
{
    if (IsKeyPressed(KEY_P))
        pantalla = 1;
}

void Juego::actualizarGameOver()
{
    if (IsKeyPressed(KEY_R))
    {
        pantalla = 4;
        nodoPelicula = historial.obtenerPrimero();
        tiempoPelicula = 0.0f;
        peliculaPausada = false;
        velocidadPelicula = 0.15f;
        cargarFotogramaPelicula();
    }
}

void Juego::actualizarComodin()
{
    Vector2 raton = GetMousePosition();
    char opciones[7] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (int i = 0; i < 7; i++)
        {
            Rectangle btn = {150.0f + (i * 80), 300.0f, 60.0f, 60.0f};
            if (CheckCollisionPointRec(raton, btn))
            {
                // Guarda la pieza elegida para la siguiente caida
                piezaComodinReservada = opciones[i];
                pantalla = 1;
            }
        }
    }
}

void Juego::dibujarComodin()
{
    dibujarJugando();
    DrawRectangle(0, 0, 900, 700, Color{0, 0, 0, 220});

    int wComodin = MeasureText("PIEZA COMODIN", 50);
    DrawText("PIEZA COMODIN", 450 - wComodin / 2, 100, 50, ORANGE);

    int wSub = MeasureText("Elige la proxima pieza que vas a recibir:", 25);
    DrawText("Elige la proxima pieza que vas a recibir:", 450 - wSub / 2, 200, 25, RAYWHITE);

    char opciones[7] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
    for (int i = 0; i < 7; i++)
    {
        Rectangle btn = {150.0f + (i * 80), 300.0f, 60.0f, 60.0f};
        DrawRectangleRec(btn, DARKGRAY);
        DrawRectangleLinesEx(btn, 2, LIGHTGRAY);
        DrawText(TextFormat("%c", opciones[i]), 150 + (i * 80) + 20, 315, 30, YELLOW);
    }
}

void Juego::actualizarJugando(float deltaTime)
{
    jugadorActual.sumarTiempo(deltaTime);
    bool hizoMovimiento = false;

    // 1. Alerta y tiempo de eventos
    if (temporizadorAlerta > 0)
        temporizadorAlerta -= deltaTime;

    if (controlesInvertidos && jugadorActual.getTiempoPartida() >= finControlesInvertidos)
    {
        controlesInvertidos = false;
    }

    // 2. Caida de piezas independiente para espejo y normal
    tiempoCaida += deltaTime;
    if (tiempoCaida >= velocidadCaida)
    {
        tiempoCaida = 0.0f;
        bool normalLock = false;
        bool espejoLock = false;

        if (!principalBloqueado)
        {
            if (!moverPieza(piezaActual, 0, 1, tablero))
                normalLock = true;
        }
        if (piezaEspejoActiva && !espejoBloqueado)
        {
            if (!moverPieza(piezaEspejo, 0, 1, tablero))
                espejoLock = true;
        }

        if (normalLock || espejoLock)
        {

            // Colocar en el tablero independiente
            if (normalLock && !principalBloqueado)
            {
                for (int bloque = 0; bloque < 4; bloque++)
                {
                    int col = obtenerXBloque(piezaActual, bloque);
                    int fila = obtenerYBloque(piezaActual, bloque);
                    tablero.colocarCelda(fila, col, obtenerIndice(piezaActual.tipo) + 1);
                }
                principalBloqueado = true;
            }
            if (espejoLock && !espejoBloqueado && piezaEspejoActiva)
            {
                for (int bloque = 0; bloque < 4; bloque++)
                {
                    int col = obtenerXBloque(piezaEspejo, bloque);
                    int fila = obtenerYBloque(piezaEspejo, bloque);
                    tablero.colocarCelda(fila, col, obtenerIndice(piezaEspejo.tipo) + 1);
                }
                espejoBloqueado = true;
            }

            // Verificar si el turno termino por completo
            if ((piezaEspejoActiva && principalBloqueado && espejoBloqueado) || (!piezaEspejoActiva && principalBloqueado))
            {

                // Bomba limpiar dos filas
                if (bombaActiva)
                {
                    tablero.eliminarFila(19);
                    tablero.insertarFilaVaciaInicio();
                    tablero.eliminarFila(19);
                    tablero.insertarFilaVaciaInicio();
                    jugadorActual.sumarLineas(2);
                    jugadorActual.sumarPuntaje(100);
                    bombaActiva = false;
                }

                // Limpieza normal de filas
                int lineasBorradas = tablero.limpiarFilas();
                if (lineasBorradas > 0)
                {
                    jugadorActual.sumarLineas(lineasBorradas);
                    jugadorActual.actualizarMaxCombo(lineasBorradas);

                    const int PUNTAJE_BASE = 50;
                    int puntos = 0;

                    if (lineasBorradas == 1)
                        puntos = PUNTAJE_BASE;
                    else if (lineasBorradas == 2)
                        puntos = (2 * PUNTAJE_BASE) + (PUNTAJE_BASE / 2);
                    else if (lineasBorradas == 3)
                        puntos = (3 * PUNTAJE_BASE) + PUNTAJE_BASE;
                    else if (lineasBorradas >= 4)
                        puntos = (lineasBorradas * PUNTAJE_BASE) * 2;

                    jugadorActual.sumarPuntaje(puntos);
                }

                // Generar siguiente pieza
                if (piezaComodinReservada != ' ')
                {
                    piezaActual = crearPieza(piezaComodinReservada);
                    piezaComodinReservada = ' ';
                }
                else
                {
                    piezaActual = crearPieza(cola.desencolar());
                    cola.rellenarSiEsNecesario();
                }

                // Reiniciar banderas
                piezaEspejoActiva = false;
                principalBloqueado = false;
                espejoBloqueado = false;

                // Activar evento solo cuando nace una nueva pieza, para evitar que se active en medio de un turno
                if (!eventos.estaVacia() && eventos.verFrente().tiempoActivacion <= jugadorActual.getTiempoPartida())
                {
                    ejecutarEvento(eventos.desencolar());
                }

                if (!posicionValida(piezaActual, tablero))
                {
                    pantalla = 3;
                    gestorArchivos.guardarPuntaje(jugadorActual.getNombre(), jugadorActual.getPuntaje(), metodoOrdenamiento);
                }
                hold.desbloquear();
            }
            hizoMovimiento = true;
        }
        else
        {
            hizoMovimiento = true;
        }
    }

    // Movimiento lateral y rotación invertido
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
    {
        tiempoMovLateral += deltaTime;
    }
    else
    {
        tiempoMovLateral = 0.0f;
    }

    bool intentoIzquierda = false;
    bool intentoDerecha = false;
    bool intentoRotar = false;
    bool intentoHardDrop = false;
    bool intentoBajar = false;

    if (controlesInvertidos)
    {
        if (IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && tiempoMovLateral >= retardoMovimiento))
        {
            intentoIzquierda = true;
        }
        if (IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && tiempoMovLateral >= retardoMovimiento))
        {
            intentoDerecha = true;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            intentoRotar = true;
        }
        if (IsKeyPressed(KEY_UP))
        {
            intentoHardDrop = true;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && tiempoMovLateral >= retardoMovimiento))
            {
                intentoIzquierda = true;
            }
            if (IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && tiempoMovLateral >= retardoMovimiento))
            {
                intentoDerecha = true;
            }
            if (IsKeyPressed(KEY_UP))
            {
                intentoRotar = true;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                intentoBajar = true;
            }
        }

        if (intentoIzquierda)
        {
                if (!principalBloqueado)
                    moverPieza(piezaActual, -1, 0, tablero);
                if (piezaEspejoActiva && !espejoBloqueado)
                    moverPieza(piezaEspejo, 1, 0, tablero); // Opuesto
                tiempoMovLateral = 0.0f;
                hizoMovimiento = true;
            }
            if (intentoDerecha)
            {
                if (!principalBloqueado)
                    moverPieza(piezaActual, 1, 0, tablero);
                if (piezaEspejoActiva && !espejoBloqueado)
                    moverPieza(piezaEspejo, -1, 0, tablero); // Opuesto
                tiempoMovLateral = 0.0f;
                hizoMovimiento = true;
            }
            if (intentoRotar)
            {
                if (!principalBloqueado)
                    rotarPieza(piezaActual, tablero);
                if (piezaEspejoActiva && !espejoBloqueado)
                {
                    // Rotar 3 veces = dirección contraria
                    rotarPieza(piezaEspejo, tablero);
                    rotarPieza(piezaEspejo, tablero);
                    rotarPieza(piezaEspejo, tablero);
                }
                hizoMovimiento = true;
            }
            if (intentoHardDrop)
            {
                if (!principalBloqueado)
                    while (moverPieza(piezaActual, 0, 1, tablero))
                        ;
                if (piezaEspejoActiva && !espejoBloqueado)
                    while (moverPieza(piezaEspejo, 0, 1, tablero))
                        ;
                tiempoCaida = velocidadCaida; // Forzar para colocar instantáneamente
                hizoMovimiento = true;
            }
            if (intentoBajar)
            {
                velocidadCaida = 0.05f;
                hizoMovimiento = true;
            }
            else
            {
                velocidadCaida = 0.5f;
            }

            // Cambio de pieza con Hold
            if (IsKeyPressed(KEY_C) && hold.puedeIntercambiar() && !principalBloqueado)
            {
                if (hold.estaVacia())
                {
                    hold.apilar(piezaActual.tipo);
                    if (piezaComodinReservada != ' ')
                    {
                        piezaActual = crearPieza(piezaComodinReservada);
                        piezaComodinReservada = ' ';
                    }
                    else
                    {
                        piezaActual = crearPieza(cola.desencolar());
                        cola.rellenarSiEsNecesario();
                    }
                }
                else
                {
                    char guardada = hold.desapilar();
                    hold.apilar(piezaActual.tipo);
                    piezaActual = crearPieza(guardada);
                }

                // El Hold desactiva los efectos de piezas raras para evitar bugs
                piezaEspejoActiva = false;
                bombaActiva = false;

                hold.bloquear();
                hizoMovimiento = true;
            }

            // Controles de deshacer y rehacer
            bool intentarDeshacer = false;
            bool intentarRehacer = false;

            if (IsKeyPressed(KEY_Z))
            {
                intentarDeshacer = true;
                tiempoReplay = 0.0f;
            }
            else if (IsKeyDown(KEY_Z))
            {
                tiempoReplay += deltaTime;
                if (tiempoReplay >= retardoReplay)
                {
                    intentarDeshacer = true;
                    tiempoReplay = 0.0f;
                }
            }
            if (IsKeyPressed(KEY_X))
            {
                intentarRehacer = true;
                tiempoReplay = 0.0f;
            }
            else if (IsKeyDown(KEY_X))
            {
                tiempoReplay += deltaTime;
                if (tiempoReplay >= retardoReplay)
                {
                    intentarRehacer = true;
                    tiempoReplay = 0.0f;
                }
            }

            if (intentarDeshacer && historial.puedeDeshacer())
            {
                EstadoJuego deshacer = historial.deshacer();
                piezaActual = deshacer.piezaActual;
                for (int fila = 0; fila < 20; fila++)
                {
                    for (int col = 0; col < 10; col++)
                    {
                        tablero.colocarCelda(fila, col, deshacer.tableroRepleay[fila][col]);
                    }
                }
                if (!hold.estaVacia())
                    hold.desapilar();
                if (!deshacer.holdVacio)
                    hold.apilar(deshacer.piezaHold);
                if (deshacer.holdBloqueado)
                    hold.bloquear();
                else
                    hold.desbloquear();
                tiempoCaida = 0.0f;
                piezaEspejoActiva = false;
                bombaActiva = false;
                principalBloqueado = false;
            }

            if (intentarRehacer && historial.puedeRehacer())
            {
                EstadoJuego rehacer = historial.rehacer();
                piezaActual = rehacer.piezaActual;
                for (int fila = 0; fila < 20; fila++)
                {
                    for (int col = 0; col < 10; col++)
                    {
                        tablero.colocarCelda(fila, col, rehacer.tableroRepleay[fila][col]);
                    }
                }
                if (!hold.estaVacia())
                {
                    hold.desapilar();
                }
                if (!rehacer.holdVacio)
                {
                    hold.apilar(rehacer.piezaHold);
                }
                if (rehacer.holdBloqueado)
                {
                    hold.bloquear();
                }
                else
                {
                    hold.desbloquear();
                }
                tiempoCaida = 0.0f;
                piezaEspejoActiva = false;
                bombaActiva = false;
                principalBloqueado = false;
            }

            if (hizoMovimiento)
            {
                historial.registrarEstado(piezaActual, tablero, hold);
            }

            if (IsKeyPressed(KEY_P))
            {
                pantalla = 2;
            }
        }

        void Juego::dibujarInicio()
        {
            DrawText("TETRIS UNA", 330, 150, 40, RAYWHITE);
            DrawText(TextFormat("Jugador actual: %s", jugadorActual.getNombre().c_str()), 330, 220, 20, LIGHTGRAY);

            Rectangle btnJugar = {350, 300, 200, 50};
            Rectangle btnJugador = {350, 380, 200, 50};
            Rectangle btnStats = {350, 460, 200, 50};

            DrawRectangleRec(btnJugar, GREEN);
            DrawText("JUGAR", 415, 315, 20, BLACK);

            DrawRectangleRec(btnJugador, BLUE);
            DrawText("Jugador", 410, 395, 20, WHITE);

            DrawRectangleRec(btnStats, ORANGE);
            DrawText("Top Jugadores", 380, 475, 20, BLACK);
        }

        void Juego::dibujarJugando()
        {
            DrawText("TETRIS", 50, 60, 50, RAYWHITE);
            // DrawText("Estructuras de Datos", 30, 120, 24, GRAY);

            DrawText("TABLERO", 390, 20, 28, RAYWHITE);

            tablero.dibujar(320, 60, 28);

            // Dibujar piezas y efectos
            if (!principalBloqueado)
            {
                dibujarPieza(piezaActual, 320, 60, 28);
                if (bombaActiva)
                {
                    for (int bloque = 0; bloque < 4; bloque++)
                    {
                        int c = obtenerXBloque(piezaActual, bloque);
                        int f = obtenerYBloque(piezaActual, bloque);
                        DrawRectangle(320 + c * 28, 60 + f * 28, 28, 28, Color{255, 0, 0, 150}); // Brillo rojo
                    }
                }
            }

            if (piezaEspejoActiva && !espejoBloqueado)
            {
                dibujarPieza(piezaEspejo, 320, 60, 28);
                for (int bloque = 0; bloque < 4; bloque++)
                {
                    int c = obtenerXBloque(piezaEspejo, bloque);
                    int f = obtenerYBloque(piezaEspejo, bloque);
                    DrawRectangle(320 + c * 28, 60 + f * 28, 28, 28, Color{200, 0, 255, 100}); // Brillo violeta
                }
            }

            // Indicadores de evento
            if (temporizadorAlerta > 0)
            {
                int wAlerta = MeasureText(textoAlerta.c_str(), 30);
                DrawText(textoAlerta.c_str(), 450 - wAlerta / 2, 640, 30, YELLOW);
            }
            if (controlesInvertidos)
            {
                DrawText("¡CONTROLES INVERTIDOS ACTIVOS!", 50, 460, 15, RED);
            }

            DrawText("Cambio (Tecla C)", 50, 200, 20, RAYWHITE);

            if (!hold.estaVacia())
            {
                Pieza pHold = crearPieza(hold.verPieza());
                pHold.x = 0;
                pHold.y = 0;
                dibujarPieza(pHold, 50, 240, 28);
            }

            DrawText("Controles:", 50, 330, 22, RAYWHITE);
            DrawText("[Z] Deshacer paso", 50, 360, 18, LIGHTGRAY);
            DrawText("[X] Rehacer paso", 50, 390, 18, LIGHTGRAY);
            DrawText("[P] Pausar juego", 50, 420, 18, LIGHTGRAY);

            DrawText("SIGUIENTES", 650, 250, 20, RAYWHITE);
            for (int i = 0; i < 3; i++)
            {
                Pieza pSiguiente = crearPieza(cola.verSiguiente(i));
                pSiguiente.x = 0;
                pSiguiente.y = 0;
                dibujarPieza(pSiguiente, 650, 290 + (i * 90), 28);
            }

            DrawText(TextFormat("PUNTAJE: %i", jugadorActual.getPuntaje()), 650, 40, 25, GREEN);
            if (jugadorActual.getUltimoPuntaje() > 0)
            {
                DrawText(TextFormat("+%i", jugadorActual.getUltimoPuntaje()), 650, 70, 20, YELLOW);
            }
            DrawText(TextFormat("Tiempo: %.0f seg", jugadorActual.getTiempoPartida()), 650, 140, 20, RAYWHITE);
        }

        void Juego::dibujarPausa()
        {
            dibujarJugando();
            DrawRectangle(0, 0, 900, 700, Color{0, 0, 0, 200});

            DrawText("PAUSA", 370, 200, 50, YELLOW);
            DrawText(TextFormat("Jugador: %s", jugadorActual.getNombre().c_str()), 350, 300, 25, RAYWHITE);
            DrawText(TextFormat("Puntaje Actual: %i", jugadorActual.getPuntaje()), 350, 350, 25, GREEN);
            DrawText(TextFormat("Tiempo: %.0f seg", jugadorActual.getTiempoPartida()), 350, 400, 25, RAYWHITE);

            DrawText("Presiona [P] para continuar", 300, 500, 20, GRAY);
        }

        void Juego::dibujarGameOver()
        {
            DrawText("GAME OVER", 300, 120, 50, RED);

            DrawText(TextFormat("Jugador: %s", jugadorActual.getNombre().c_str()), 300, 230, 25, RAYWHITE);
            DrawText(TextFormat("Puntaje Total: %i", jugadorActual.getPuntaje()), 300, 280, 25, GREEN);
            DrawText(TextFormat("Tiempo de Juego: %.0f seg", jugadorActual.getTiempoPartida()), 300, 330, 25, RAYWHITE);
            DrawText(TextFormat("Lineas Totales: %i", jugadorActual.getLineasTotales()), 300, 380, 25, RAYWHITE);
            DrawText(TextFormat("Mejor Combo: %i lineas", jugadorActual.getMaxLineasCombo()), 300, 430, 25, YELLOW);

            DrawText("Presiona [R] para ver el Replay", 250, 550, 25, LIGHTGRAY);
        }

        void Juego::cargarFotogramaPelicula()
        {
            if (nodoPelicula != nullptr)
            {
                piezaActual = nodoPelicula->estado.piezaActual;
                for (int fila = 0; fila < 20; fila++)
                {
                    for (int col = 0; col < 10; col++)
                    {
                        tablero.colocarCelda(fila, col, nodoPelicula->estado.tableroRepleay[fila][col]);
                    }
                }
            }
        }

        void Juego::actualizarPelicula(float deltaTime)
        {
            Vector2 raton = GetMousePosition();

            Rectangle btnAtras = {85, 620, 130, 40};
            Rectangle btnPausa = {235, 620, 130, 40};
            Rectangle btnAdelante = {385, 620, 130, 40};
            Rectangle btnVelocidad = {535, 620, 130, 40};
            Rectangle btnSalir = {685, 620, 130, 40};

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                tiempoReplay += deltaTime;
                if (tiempoReplay >= 0.05f)
                {
                    if (CheckCollisionPointRec(raton, btnAtras))
                    {
                        if (nodoPelicula != nullptr && nodoPelicula->anterior != nullptr)
                        {
                            nodoPelicula = nodoPelicula->anterior;
                            cargarFotogramaPelicula();
                            peliculaPausada = true;
                        }
                        tiempoReplay = 0.0f;
                    }
                    else if (CheckCollisionPointRec(raton, btnAdelante))
                    {
                        if (nodoPelicula != nullptr && nodoPelicula->siguiente != nullptr)
                        {
                            nodoPelicula = nodoPelicula->siguiente;
                            cargarFotogramaPelicula();
                            peliculaPausada = true;
                        }
                        tiempoReplay = 0.0f;
                    }
                }
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(raton, btnPausa))
                    peliculaPausada = !peliculaPausada;
                else if (CheckCollisionPointRec(raton, btnVelocidad))
                {
                    if (velocidadPelicula == 0.15f)
                        velocidadPelicula = 0.05f;
                    else
                        velocidadPelicula = 0.15f;
                }
                else if (CheckCollisionPointRec(raton, btnSalir))
                    pantalla = 3;
                    
            }

            if (!peliculaPausada)
            {
                tiempoPelicula += deltaTime;
                if (tiempoPelicula >= velocidadPelicula)
                {
                    tiempoPelicula = 0.0f;
                    if (nodoPelicula != nullptr && nodoPelicula->siguiente != nullptr)
                    {
                        nodoPelicula = nodoPelicula->siguiente;
                        cargarFotogramaPelicula();
                    }
                }
            }
        }

        void Juego::dibujarPelicula()
        {
            DrawText("REPRODUCIENDO PARTIDA...", 250, 20, 25, GREEN);

            tablero.dibujar(320, 60, 28);
            dibujarPieza(piezaActual, 320, 60, 28);

            Rectangle btnAtras = {85, 620, 130, 40};
            Rectangle btnPausa = {235, 620, 130, 40};
            Rectangle btnAdelante = {385, 620, 130, 40};
            Rectangle btnVelocidad = {535, 620, 130, 40};
            Rectangle btnSalir = {685, 620, 130, 40};

            DrawRectangleRec(btnAtras, DARKGRAY);
            DrawText("<<< Atras", 100, 630, 20, WHITE);

            DrawRectangleRec(btnPausa, peliculaPausada ? MAROON : DARKBLUE);
            DrawText(peliculaPausada ? "Reproducir" : "Pausar", 255, 630, 20, WHITE);

            DrawRectangleRec(btnAdelante, DARKGRAY);
            DrawText("Adelante >>>", 390, 630, 20, WHITE);

            DrawRectangleRec(btnVelocidad, ORANGE);
            DrawText(velocidadPelicula == 0.15f ? "Velocidad: x1" : "Velocidad: x3", 540, 630, 18, BLACK);

            DrawRectangleRec(btnSalir, RED);
            DrawText("Salir", 725, 630, 20, WHITE);
        }
