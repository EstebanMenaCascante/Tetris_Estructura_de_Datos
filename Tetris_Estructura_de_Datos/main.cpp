#include "Juego.h"
#include "Archivo.h"
#include "raylib.h"

int main(int argc, char *argv[])
{
	
	//Archivo medicion;
	//medicion.probarOrdenamientos();
	
	InitWindow(900, 700, "Tetris - Estructuras de Datos");
	SetTargetFPS(60);

	Juego tetris;

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		tetris.actualizar(deltaTime);

		BeginDrawing();
		ClearBackground(Color{18, 20, 28, 255});
		tetris.dibujar();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
