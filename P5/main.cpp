#include <iostream>
#include "Simulador.h"
#include "Vector2D.h"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  SIMULADOR DE COLISIONES MULTIPLES" << endl;
    cout << "========================================" << endl;

    // Crear simulador
    // Parámetros: ancho, alto, timestep (dt), coeficiente de restitución
    Simulador sim(100.0, 100.0, 0.01, 0.7);

    // Agregar 4 obstáculos cuadrados en las esquinas
    cout << "\nAgregando obstaculos..." << endl;
    sim.agregarObstaculo(Vector2D(25.0, 25.0), 8.0);
    sim.agregarObstaculo(Vector2D(75.0, 25.0), 8.0);
    sim.agregarObstaculo(Vector2D(25.0, 75.0), 8.0);
    sim.agregarObstaculo(Vector2D(75.0, 75.0), 8.0);

    // Agregar 6 partículas con diferentes propiedades
    cout << "Agregando particulas..." << endl;

    // Partícula 0: esquina inferior izquierda
    sim.agregarParticula(Vector2D(15.0, 15.0), Vector2D(30.0, 20.0), 1.0, 3.0);

    // Partícula 1: esquina inferior derecha
    sim.agregarParticula(Vector2D(85.0, 15.0), Vector2D(-25.0, 25.0), 1.5, 3.5);

    // Partícula 2: esquina superior izquierda
    sim.agregarParticula(Vector2D(15.0, 85.0), Vector2D(35.0, -30.0), 1.2, 3.2);

    // Partícula 3: esquina superior derecha
    sim.agregarParticula(Vector2D(85.0, 85.0), Vector2D(-20.0, -28.0), 0.8, 2.8);

    // Partícula 4: centro
    sim.agregarParticula(Vector2D(50.0, 50.0), Vector2D(15.0, -15.0), 2.0, 4.0);

    // Partícula 5: centro-inferior
    sim.agregarParticula(Vector2D(50.0, 10.0), Vector2D(0.0, 40.0), 1.0, 3.0);

    // Mostrar configuración
    sim.imprimirConfiguracion();

    // Ejecutar simulación durante 10 segundos
    sim.simular(10.0);

    cout << "\n========================================" << endl;
    cout << "           SIMULACION FINALIZADA" << endl;
    cout << "========================================" << endl;
    cout << "\nPara visualizar los resultados:" << endl;
    cout << "  1. Busca el archivo 'simulacion.txt' en la carpeta del proyecto" << endl;
    cout << "  2. Ejecuta: python visualizar.py" << endl;
    cout << "========================================" << endl;

    return 0;
}
