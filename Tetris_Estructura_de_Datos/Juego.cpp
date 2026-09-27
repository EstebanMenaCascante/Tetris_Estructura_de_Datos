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

	piezaActual = crearPieza(cola.desencolar());
	cola.rellenarSiEsNecesario();
	historial.registrarEstado(piezaActual, tablero, hold);
}

void Juego::actualizar(float deltaTime)
{
	if (pantalla == 0)
		actualizarInicio();
	else if (pantalla == 1)
		actualizarJugando(deltaTime);
	else if (pantalla == 2)
		actualizarPausa();
	else if (pantalla == 3)
		actualizarGameOver();
	else if (pantalla == 4)
		actualizarPelicula(deltaTime);
	else if (pantalla == 5)
		actualizarEscribirNombre(); // Dibujo de caja de texto
}

void Juego::dibujar()
{
	if (pantalla == 0)
		dibujarInicio();
	else if (pantalla == 1)
		dibujarJugando();
	else if (pantalla == 2)
		dibujarPausa();
	else if (pantalla == 3)
		dibujarGameOver();
	else if (pantalla == 4)
		dibujarPelicula();
	else if (pantalla == 5)
		dibujarEscribirNombre(); // Dibujo de caja de texto
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
		}
		else if (CheckCollisionPointRec(raton, btnJugador))
		{
			nombreTemp = "";
			pantalla = 5;
		}
		else if (CheckCollisionPointRec(raton, btnStats))
		{
			// falta pantalla de estadisticas
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
			nombreTemp.pop_back(); // Elimina la ultima letra
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

	// Dibujar la caja de texto
	DrawRectangle(250, 280, 400, 60, LIGHTGRAY);
	DrawText(nombreTemp.c_str(), 270, 295, 30, BLACK);

	framesCursor++;
	if (((framesCursor / 30) % 2) == 0 && nombreTemp.length() < 15)
	{
		DrawText("_", 270 + MeasureText(nombreTemp.c_str(), 30), 295, 30, BLACK);
	}

	DrawText("Presiona ENTER para guardar", 270, 400, 20, GRAY);
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

void Juego::actualizarJugando(float deltaTime)
{
	jugadorActual.sumarTiempo(deltaTime);
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
				tablero.colocarCelda(fila, col, obtenerIndice(piezaActual.tipo) + 1);
			}

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

			piezaActual = crearPieza(cola.desencolar());
			cola.rellenarSiEsNecesario();

			if (!posicionValida(piezaActual, tablero))
			{
				pantalla = 3;
			}
			hold.desbloquear();
			hizoMovimiento = true;
		}
		else
		{
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
			char guardada = hold.desapilar();
			hold.apilar(piezaActual.tipo);
			piezaActual = crearPieza(guardada);
		}
		hold.bloquear();
		hizoMovimiento = true;
	}

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
			hold.desapilar();
		if (!rehacer.holdVacio)
			hold.apilar(rehacer.piezaHold);
		if (rehacer.holdBloqueado)
			hold.bloquear();
		else
			hold.desbloquear();
		tiempoCaida = 0.0f;
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
	DrawText("TETRIS UNA", 250, 150, 40, RAYWHITE);
	DrawText(TextFormat("Jugador actual: %s", jugadorActual.getNombre().c_str()), 330, 220, 20, LIGHTGRAY);

	Rectangle btnJugar = {350, 300, 200, 50};
	Rectangle btnJugador = {350, 380, 200, 50};
	Rectangle btnStats = {350, 460, 200, 50};

	DrawRectangleRec(btnJugar, GREEN);
	DrawText("JUGAR", 415, 315, 20, BLACK);

	DrawRectangleRec(btnJugador, BLUE);
	DrawText("Agregar Jugador", 365, 395, 20, WHITE);

	DrawRectangleRec(btnStats, ORANGE);
	DrawText("Estadisticas", 390, 475, 20, BLACK);
}

void Juego::dibujarJugando()
{
	DrawText("TETRIS", 60, 60, 50, RAYWHITE);
	DrawText("Estructuras de Datos", 30, 120, 24, GRAY);
	tablero.dibujar(320, 60, 28);
	dibujarPieza(piezaActual, 320, 60, 28);

	DrawText("TABLERO", 650, 100, 28, RAYWHITE);
	DrawText("Cambio (Tecla C)", 50, 200, 20, RAYWHITE);
	DrawText("Deshacer (Tecla Z)", 50, 320, 20, RAYWHITE);
	DrawText("Rehacer (Tecla X)", 50, 380, 20, RAYWHITE);
	DrawText("Pausa (Tecla P)", 50, 440, 20, RAYWHITE);

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

	// Mostrar Puntaje y Tiempo
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
	DrawText("GAME OVER", 330, 120, 50, RED);

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
	DrawText("REPRODUCIENDO PARTIDA", 250, 20, 25, GREEN);

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
