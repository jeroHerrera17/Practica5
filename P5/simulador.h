#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Caja.h"
#include "Particula.h"
#include "Obstaculo.h"
#include "Vector2D.h"
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Simulador {
private:
    Caja caja;
    vector<Particula> particulas;
    vector<Obstaculo> obstaculos;
    double tiempo;
    double dt;
    double coefRestitucion;
    ofstream archivo;
    string rutaArchivoSalida;

public:
    // Constructor y destructor
    Simulador(double ancho, double alto, double timestep = 0.01, double coefRest = 0.7);
    ~Simulador();

    // Métodos públicos
    void agregarParticula(Vector2D pos, Vector2D vel, double masa, double radio);
    void agregarObstaculo(Vector2D pos, double lado);
    void simular(double tiempoTotal);

    // Utilidades
    void imprimirConfiguracion() const;
};

#endif // SIMULADOR_H
