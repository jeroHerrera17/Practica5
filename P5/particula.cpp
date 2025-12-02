#include "Particula.h"
#include <iostream>
#include <cmath>

using namespace std;

// Inicialización de variable estática
int Particula::nextId = 0;

// Constructor
Particula::Particula(Vector2D pos, Vector2D vel, double m, double r)
    : posicion(pos), velocidad(vel), masa(m), radio(r), activa(true) {
    id = nextId++;
}

// Métodos públicos
int Particula::getId() const {
    return id;
}

void Particula::actualizar(double dt) {
    if (activa) {
        // Actualizar posición usando velocidad
        // x(t + dt) = x(t) + v * dt
        posicion = posicion + velocidad * dt;
    }
}

void Particula::fusionarCon(Particula& otra) {
    if (!activa || !otra.activa) {
        return;
    }

    // Conservación del momento lineal
    // p_total = m1*v1 + m2*v2
    Vector2D momentoTotal = velocidad * masa + otra.velocidad * otra.masa;
    double masaTotal = masa + otra.masa;

    // Nueva velocidad del sistema fusionado
    // v' = p_total / m_total
    velocidad = momentoTotal / masaTotal;

    // Nueva posición (centro de masa)
    // x_cm = (m1*x1 + m2*x2) / (m1 + m2)
    posicion = (posicion * masa + otra.posicion * otra.masa) / masaTotal;

    // Nueva masa
    masa = masaTotal;

    // Nuevo radio (conservando área total)
    // A_total = A1 + A2
    // π*r'^2 = π*r1^2 + π*r2^2
    // r' = sqrt(r1^2 + r2^2)
    radio = sqrt(radio * radio + otra.radio * otra.radio);

    // Desactivar la partícula absorbida
    otra.activa = false;
}

// Utilidades
void Particula::imprimirInfo() const {
    cout << "Particula #" << id << ": ";
    cout << "Pos=";
    posicion.print();
    cout << ", Vel=";
    velocidad.print();
    cout << ", Masa=" << masa << ", Radio=" << radio;
    cout << ", Activa=" << (activa ? "Si" : "No") << endl;
}
