#include "Caja.h"
#include <iostream>

using namespace std;

// Constructor
Caja::Caja(double w, double h) : ancho(w), alto(h) {}

// Utilidades
void Caja::imprimirInfo() const {
    cout << "Caja: " << ancho << " x " << alto << " unidades" << endl;
}
