#include "Obstaculo.h"
#include <iostream>

using namespace std;

// Constructor
Obstaculo::Obstaculo(Vector2D pos, double l) : posicion(pos), lado(l) {}

// Métodos para obtener límites del obstáculo
double Obstaculo::getMinX() const {
    return posicion.x - lado / 2.0;
}

double Obstaculo::getMaxX() const {
    return posicion.x + lado / 2.0;
}

double Obstaculo::getMinY() const {
    return posicion.y - lado / 2.0;
}

double Obstaculo::getMaxY() const {
    return posicion.y + lado / 2.0;
}

// Utilidades
void Obstaculo::imprimirInfo() const {
    cout << "Obstaculo: Centro=";
    posicion.print();
    cout << ", Lado=" << lado;
    cout << ", Limites=[" << getMinX() << ", " << getMaxX()
         << "] x [" << getMinY() << ", " << getMaxY() << "]" << endl;
}
