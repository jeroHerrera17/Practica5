#ifndef CAJA_H
#define CAJA_H

class Caja {
public:
    double ancho;
    double alto;

    // Constructor
    Caja(double w, double h);

    // Utilidades
    void imprimirInfo() const;
};

#endif // CAJA_H
