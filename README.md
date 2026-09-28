# Tetris - Estructuras de Datos

Proyecto académico de **Estructuras de Datos** de la Universidad Nacional de Costa Rica (UNA). Es una versión de Tetris desarrollada en C++ que utiliza estructuras de datos lineales implementadas dentro del proyecto y una interfaz gráfica basada en Raylib.

El objetivo es aplicar colas, pilas, listas enlazadas y algoritmos de ordenamiento a las mecánicas principales del juego, manteniendo una solución clara y fácil de explicar durante la defensa.

## Características principales

- Tablero de 10 columnas por 20 filas.
- Las siete piezas clásicas: `I`, `O`, `T`, `S`, `Z`, `J` y `L`.
- Caída automática, movimiento horizontal, rotación y hard drop.
- Visualización de las próximas tres piezas.
- Pieza en espera mediante la opción Hold.
- Detección y eliminación de una o varias líneas completas.
- Animación visual sencilla para la caída de piezas.
- Animación de parpadeo antes de eliminar líneas.
- Sistema de puntaje, líneas totales y mejor combo.
- Pantalla de pausa y pantalla de Game Over.
- Replay con avance, retroceso, pausa y cambio de velocidad.
- Tabla de mejores puntajes guardada en `puntajes.txt`.
- Eventos especiales programados durante la partida.

## Estructuras de datos utilizadas

| Módulo | Responsabilidad |
|---|---|
| `ColaPiezas` | Cola propia para administrar las piezas siguientes. Genera bolsas de las siete piezas y permite mostrar las próximas piezas. |
| `PilaHold` | Pila propia de capacidad uno para guardar e intercambiar la pieza en espera. |
| `ListaReplay` | Lista doblemente enlazada para registrar estados, deshacer, rehacer y reproducir la partida. |
| `ColaEventos` | Cola enlazada ordenada por el momento de activación de cada evento. |
| `Tablero` | Lista enlazada de 20 filas. Cada fila contiene 10 celdas y puede eliminarse o insertarse al limpiar líneas. |
| `Archivo` | Lectura, escritura y ordenamiento de la tabla de mejores puntajes. |
| `Juego` | Coordina las reglas, entradas del jugador, eventos, pantallas, animaciones y dibujo. |
| `Pieza` | Define las formas, orientaciones, colisiones y representación visual de los tetrominós. |

No se utilizan `std::queue`, `std::stack`, `std::deque`, `std::list`, `std::priority_queue` ni `std::sort` para las estructuras evaluadas por el proyecto. Se utiliza `std::vector` como colección auxiliar para cargar y ordenar los registros de puntajes.

## Eventos especiales

Los eventos se programan en una cola ordenada y se activan durante la partida. La implementación actual incluye:

- **Controles invertidos:** invierte temporalmente el movimiento horizontal y algunos controles.
- **Pieza espejo:** crea una segunda pieza que cae y se mueve de forma opuesta.
- **Pieza bomba:** elimina dos filas al colocarse y otorga un puntaje adicional.
- **Pieza comodín:** permite elegir la próxima pieza entre las siete piezas disponibles.

Los avisos de eventos aparecen temporalmente en la interfaz. La pieza bomba reutiliza la transición visual de eliminación de líneas.

## Interfaz y animaciones

La ventana principal tiene una resolución de `900 x 700` píxeles. La interfaz muestra el tablero, la pieza activa, Hold, las próximas piezas, el puntaje, el tiempo de partida y los avisos de eventos.

Las animaciones se mantienen deliberadamente sencillas:

- La posición lógica de la pieza continúa usando filas y columnas enteras.
- Durante una fracción de segundo, la pieza se dibuja con un desplazamiento vertical entre su fila anterior y la nueva.
- Las filas completas permanecen visibles y parpadean antes de que `Tablero` las elimine.
- Mientras dura la animación de líneas no se procesa una nueva pieza ni controles del jugador.

## Controles

### Durante la partida

| Tecla | Acción |
|---|---|
| Flecha izquierda | Mover a la izquierda |
| Flecha derecha | Mover a la derecha |
| Flecha abajo | Acelerar la caída |
| Flecha arriba | Rotacion |
| `C` | Usar o intercambiar Hold |
| `P` | Pausar o continuar |
| `Z` | Deshacer un estado |
| `X` | Rehacer un estado |

Los controles de movimiento se adaptan automáticamente cuando está activo el evento de controles invertidos.

### En el replay

El replay se abre desde la pantalla de Game Over presionando `R`.

- **Atrás:** retrocede un estado.
- **Reproducir/Pausar:** controla la reproducción automática.
- **Adelante:** avanza un estado.
- **Velocidad:** cambia la velocidad de reproducción.
- **Salir:** vuelve a la pantalla inicial.

## Pantallas disponibles

- **Inicio:** iniciar una partida, cambiar el nombre del jugador o consultar estadísticas.
- **Juego:** tablero, piezas, información de partida y controles.
- **Pausa:** conserva el estado actual y permite continuar.
- **Pieza comodín:** selección de la próxima pieza durante el evento correspondiente.
- **Estadísticas:** tabla de mejores puntajes y selección del método de ordenamiento.
- **Game Over:** resumen de la partida y acceso al replay.
- **Replay:** reproducción completa del historial registrado.

## Requisitos

- Windows.
- C++
- [ZinjaI](https://zinjai.sourceforge.net/).
- [Raylib](https://www.raylib.com/) configurada para C++.

El archivo de proyecto `Tetris_Estructura_de_Datos/Tetris_Estructura_de_Datos.zpr` incluye la configuración utilizada para ZinjaI. En la configuración Debug se espera encontrar Raylib en:

```text
C:\raylib\w64devkit\include
C:\raylib\w64devkit\lib
```

Si Raylib está instalada en otra ubicación, se deben actualizar las rutas de encabezados y bibliotecas en la configuración del proyecto.

## Compilación y ejecución con ZinjaI

1. Instalar Raylib y ZinjaI.
2. Abrir `Tetris_Estructura_de_Datos/Tetris_Estructura_de_Datos.zpr` en ZinjaI.
3. Verificar las rutas de include y libraries de Raylib.
4. Seleccionar la configuración `Debug` o `Release`.
5. Compilar y ejecutar el proyecto.

El punto de entrada del programa se encuentra en `Tetris_Estructura_de_Datos/main.cpp`.

## Organización del proyecto

```text
Tetris_Estructura_de_Datos/
├── Archivo.cpp / Archivo.h
├── ColaEventos.cpp / ColaEventos.h
├── ColaPiezas.cpp / ColaPiezas.h
├── Juego.cpp / Juego.h
├── Jugador.cpp / Jugador.h
├── ListaReplay.cpp / ListaReplay.h
├── PilaHold.cpp / PilaHold.h
├── Pieza.cpp / Pieza.h
├── Tablero.cpp / Tablero.h
├── main.cpp
├── puntajes.txt
└── Tetris_Estructura_de_Datos.zpr
```

## Puntajes y estadísticas

Las líneas completadas actualizan las estadísticas del jugador y generan puntaje según la cantidad de líneas eliminadas. Al finalizar una partida, el registro se guarda en `puntajes.txt`.

La pantalla de estadísticas permite escoger entre dos métodos implementados en `Archivo`:

- Insertion Sort, con complejidad aproximada `O(n^2)`.
- Merge Sort, con complejidad aproximada `O(n log n)`.

## Nota sobre el alcance

Este README resume el funcionamiento y la organización del código. La justificación formal de complejidades, las mediciones experimentales, el diagrama de estructuras y las respuestas de análisis del enunciado deben presentarse en el informe académico correspondiente.

## Autoría

Proyecto desarrollado para el curso **Estructuras de Datos** por el estudiante **Esteban Josué Mena Cascante**, en el año **2026**.