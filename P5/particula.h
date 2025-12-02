#ifndef PARTICULA_H
#define PARTICULA_H

#include "Vector2D.h"

class Particula {
private:
    static int nextId;
    int id;

public:
    Vector2D posicion;
    Vector2D velocidad;
    double masa;
    double radio;
    bool activa;

    // Constructor
    Particula(Vector2D pos, Vector2D vel, double m, double r);

    // Métodos públicos
    int getId() const;
    void actualizar(double dt);
    void fusionarCon(Particula& otra);

    // Utilidades
    void imprimirInfo() const;
};

#endif // PARTICULA_H
