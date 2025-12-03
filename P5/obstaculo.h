#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Vector2D.h"

class Obstaculo {
public:
    Vector2D posicion;  // Centro del obstáculo
    double lado;

    // Constructor
    Obstaculo(Vector2D pos, double l);

    // Métodos para obtener límites del obstáculo
    double getMinX() const;
    double getMaxX() const;
    double getMinY() const;
    double getMaxY() const;

    // Utilidades
    void imprimirInfo() const;
};

#endif // OBSTACULO_H
