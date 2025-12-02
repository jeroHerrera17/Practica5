#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D {
public:
    double x, y;

    // Constructores
    Vector2D();
    Vector2D(double x, double y);

    // Operadores sobrecargados
    Vector2D operator+(const Vector2D& v) const;
    Vector2D operator-(const Vector2D& v) const;
    Vector2D operator*(double scalar) const;
    Vector2D operator/(double scalar) const;

    // Operaciones vectoriales
    double dot(const Vector2D& v) const;
    double magnitude() const;
    Vector2D normalize() const;

    // Utilidades
    void print() const;
};

#endif // VECTOR2D_H
