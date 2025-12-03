#include "Simulador.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#ifdef _WIN32
//include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

using namespace std;

// Constructor
Simulador::Simulador(double ancho, double alto, double timestep, double coefRest)
    : caja(ancho, alto), tiempo(0.0), dt(timestep), coefRestitucion(coefRest) {

    // Simplemente guarda en el directorio actual
    rutaArchivoSalida = "simulacion.txt";


    // Abrir archivo de salida
    archivo.open(rutaArchivoSalida.c_str());

    if (!archivo.is_open()) {
        cerr << "ERROR: No se pudo crear el archivo de salida!" << endl;
        cerr << "Intentando crear en: " << rutaArchivoSalida << endl;
        return;
    }

    cout << "\n[INFO] Archivo de salida: " << rutaArchivoSalida << endl;

    // Escribir encabezado
    archivo << "# Simulacion de Colisiones Multiples" << endl;
    archivo << "# Formato: tiempo,tipo_evento,id_particula,..." << endl;
    archivo << "# CAJA: " << ancho << " x " << alto << endl;
    archivo << "# DT: " << dt << endl;
    archivo << "# COEF_RESTITUCION: " << coefRestitucion << endl;
}

// Destructor
Simulador::~Simulador() {
    if (archivo.is_open()) {
        archivo.close();
    }
}

// Métodos públicos
void Simulador::agregarParticula(Vector2D pos, Vector2D vel, double masa, double radio) {
    particulas.push_back(Particula(pos, vel, masa, radio));
}

void Simulador::agregarObstaculo(Vector2D pos, double lado) {
    obstaculos.push_back(Obstaculo(pos, lado));
    archivo << "# OBSTACULO: " << pos.x << "," << pos.y << "," << lado << endl;
}
