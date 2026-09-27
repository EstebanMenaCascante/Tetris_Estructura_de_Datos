#include "Juego.h"

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
}

void Juego::dibujar()
{
	if (pantalla == 0)
		dibujarInicio();
	else if (pantalla == 1 || pantalla == 2)
		dibujarJugando();
	else if (pantalla == 3)
		dibujarGameOver();
	else if (pantalla == 4)
		dibujarPelicula();
}

void Juego::actualizarInicio()
{
	if (IsKeyPressed(KEY_ENTER))
		pantalla = 1;
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
			tablero.limpiarFilas();
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
	DrawText("TETRIS ESTUDIANTIL", 250, 300, 40, RAYWHITE);
	DrawText("Presiona ENTER para iniciar", 280, 400, 20, GRAY);
}

void Juego::dibujarJugando()
{
	DrawText("TETRIS", 60, 60, 50, RAYWHITE);
	DrawText("Estructuras de Datos", 30, 120, 24, GRAY);
	tablero.dibujar(320, 60, 28);
	dibujarPieza(piezaActual, 320, 60, 28);

	DrawText("TABLERO", 650, 100, 28, RAYWHITE);
	DrawText("Cambio (Tecla C)", 50, 200, 20, RAYWHITE);

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

	if (pantalla == 2)
	{
		DrawText("PAUSA", 400, 300, 40, YELLOW);
	}
}

void Juego::dibujarGameOver()
{
	DrawText("GAME OVER", 300, 250, 50, RED);
	DrawText("Presiona [R] para ver la Repeticion", 260, 350, 20, RAYWHITE);
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
		{
			peliculaPausada = !peliculaPausada;
		}
		else if (CheckCollisionPointRec(raton, btnVelocidad))
		{
			if (velocidadPelicula == 0.15f)
				velocidadPelicula = 0.05f; // Modo x3
			else
				velocidadPelicula = 0.15f; // Modo Normal
		}
		else if (CheckCollisionPointRec(raton, btnSalir))
		{
			pantalla = 3;
		}
	}

	// Reproduccion automatica
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
	DrawText("REPRODUCIENDO JUEGO", 250, 20, 25, GREEN);

	// Dibujar el tablero y la pieza actual
	tablero.dibujar(320, 60, 28);
	dibujarPieza(piezaActual, 320, 60, 28);

	// Dibujar los botones de control
	Rectangle btnAtras = {85, 620, 130, 40};
	Rectangle btnPausa = {235, 620, 130, 40};
	Rectangle btnAdelante = {385, 620, 130, 40};
	Rectangle btnVelocidad = {535, 620, 130, 40};
	Rectangle btnSalir = {685, 620, 130, 40};

	// Dibujar los botones con colores y texto
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
