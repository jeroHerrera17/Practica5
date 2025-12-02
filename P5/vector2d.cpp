#include "Vector2D.h"
#include <iostream>
#include <cmath>

using namespace std;

// Constructores
Vector2D::Vector2D() : x(0.0), y(0.0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

// Operadores sobrecargados
Vector2D Vector2D::operator+(const Vector2D& v) const {
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
    return Vector2D(x - v.x, y - v.y);
}

Vector2D Vector2D::operator*(double scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(double scalar) const {
    if (scalar != 0.0) {
        return Vector2D(x / scalar, y / scalar);
    }
    return Vector2D(0.0, 0.0);
}

// Operaciones vectoriales
double Vector2D::dot(const Vector2D& v) const {
    return x * v.x + y * v.y;
}

double Vector2D::magnitude() const {
    return sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() const {
    double mag = magnitude();
    if (mag > 0.0) {
        return *this / mag;
    }
    return Vector2D(0.0, 0.0);
}

// Utilidades
void Vector2D::print() const {
    cout << "(" << x << ", " << y << ")";
}
