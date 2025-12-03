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

void Simulador::simular(double tiempoTotal) {
    int pasos = static_cast<int>(tiempoTotal / dt);

    cout << "\n=== INICIANDO SIMULACION ===" << endl;
    cout << "Tiempo total: " << tiempoTotal << " segundos" << endl;
    cout << "Pasos de tiempo: " << pasos << endl;
    cout << "dt: " << dt << " segundos" << endl;
    cout << "Particulas: " << particulas.size() << endl;
    cout << "Obstaculos: " << obstaculos.size() << endl;
    cout << "\nProgreso:" << endl;

    for (int paso = 0; paso <= pasos; paso++) {
        // Guardar posiciones actuales
        guardarPosiciones();

        // Detectar y manejar colisiones entre partículas
        for (size_t i = 0; i < particulas.size(); i++) {
            for (size_t j = i + 1; j < particulas.size(); j++) {
                if (detectarColisionParticulas(particulas[i], particulas[j])) {
                    manejarColisionParticulas(particulas[i], particulas[j]);
                }
            }
        }

        // Detectar y manejar colisiones con obstáculos
        for (size_t i = 0; i < particulas.size(); i++) {
            if (!particulas[i].activa) continue;

            for (size_t j = 0; j < obstaculos.size(); j++) {
                manejarColisionObstaculo(particulas[i], obstaculos[j]);
            }
        }

        // Detectar y manejar colisiones con paredes
        for (size_t i = 0; i < particulas.size(); i++) {
            if (particulas[i].activa) {
                manejarColisionPared(particulas[i]);
            }
        }

        // Actualizar posiciones de todas las partículas activas
        for (size_t i = 0; i < particulas.size(); i++) {
            particulas[i].actualizar(dt);
        }

        // Incrementar tiempo
        tiempo += dt;

        // Mostrar progreso cada 10%
        if (pasos > 0 && paso % (pasos / 10) == 0) {
            int porcentaje = static_cast<int>(100.0 * paso / pasos);
            cout << porcentaje << "% completado..." << endl;
        }
    }

    cout << "\n=== SIMULACION COMPLETADA ===" << endl;
    cout << "Datos guardados en: " << rutaArchivoSalida << endl;

// Obtener ruta absoluta para mostrar al usuario
#ifdef _WIN32
    char fullPath[_MAX_PATH];
    if (_fullpath(fullPath, rutaArchivoSalida.c_str(), _MAX_PATH)) {
        cout << "Ruta completa: " << fullPath << endl;
    }
#else
    char fullPath[PATH_MAX];
    if (realpath(rutaArchivoSalida.c_str(), fullPath)) {
        cout << "Ruta completa: " << fullPath << endl;
    }
#endif
}

// Métodos privados de detección
bool Simulador::detectarColisionParticulas(const Particula& p1, const Particula& p2) {
    // No detectar colisión si alguna está inactiva o son la misma
    if (!p1.activa || !p2.activa || p1.getId() == p2.getId()) {
        return false;
    }

    // Calcular distancia entre centros
    Vector2D diferencia = p1.posicion - p2.posicion;
    double distancia = diferencia.magnitude();

    // Hay colisión si la distancia es menor que la suma de radios
    return distancia < (p1.radio + p2.radio);
}

bool Simulador::detectarColisionObstaculo(const Particula& p, const Obstaculo& obs,
                                          Vector2D& puntoColision, Vector2D& normal) {
    if (!p.activa) return false;

    // Encontrar el punto más cercano del cuadrado al círculo
    double closestX = max(obs.getMinX(), min(p.posicion.x, obs.getMaxX()));
    double closestY = max(obs.getMinY(), min(p.posicion.y, obs.getMaxY()));

    Vector2D puntoMasCercano(closestX, closestY);
    Vector2D distancia = p.posicion - puntoMasCercano;
    double dist = distancia.magnitude();

    // Hay colisión si la distancia es menor que el radio
    if (dist < p.radio) {
        puntoColision = puntoMasCercano;

        // Determinar la normal según el lado del cuadrado
        double dx1 = abs(p.posicion.x - obs.getMinX());
        double dx2 = abs(p.posicion.x - obs.getMaxX());
        double dy1 = abs(p.posicion.y - obs.getMinY());
        double dy2 = abs(p.posicion.y - obs.getMaxY());

        double minDist = min({dx1, dx2, dy1, dy2});

        // Asignar normal según el lado más cercano
        if (minDist == dx1) {
            normal = Vector2D(-1.0, 0.0);  // Lado izquierdo
        } else if (minDist == dx2) {
            normal = Vector2D(1.0, 0.0);   // Lado derecho
        } else if (minDist == dy1) {
            normal = Vector2D(0.0, -1.0);  // Lado inferior
        } else {
            normal = Vector2D(0.0, 1.0);   // Lado superior
        }

        return true;
    }

    return false;
}

// Métodos privados de manejo de colisiones
void Simulador::manejarColisionParticulas(Particula& p1, Particula& p2) {
    // Registrar colisión
    registrarColision("COLISION_PARTICULAS", p1.getId(), p2.getId());

    // Fusionar partículas (colisión completamente inelástica)
    p1.fusionarCon(p2);
}

void Simulador::manejarColisionObstaculo(Particula& p, const Obstaculo& obs) {
    Vector2D puntoColision, normal;

    if (detectarColisionObstaculo(p, obs, puntoColision, normal)) {
        // Registrar colisión
        registrarColision("COLISION_OBSTACULO", p.getId());

        // Descomponer velocidad en componentes normal y paralela
        double velocidadNormal = p.velocidad.dot(normal);

        // Componente paralela (tangencial)
        Vector2D velocidadTangencial(
            p.velocidad.x - velocidadNormal * normal.x,
            p.velocidad.y - velocidadNormal * normal.y
            );

        // Aplicar coeficiente de restitución a la componente normal
        // v'_n = -ε * v_n
        velocidadNormal = -coefRestitucion * velocidadNormal;

        // Reconstruir velocidad total
        // v' = v'_n * n + v_t
        p.velocidad.x = velocidadTangencial.x + velocidadNormal * normal.x;
        p.velocidad.y = velocidadTangencial.y + velocidadNormal * normal.y;

        // Separar partícula del obstáculo para evitar solapamiento
        Vector2D separacion = normal * (p.radio - (p.posicion - puntoColision).magnitude() + 0.1);
        p.posicion = p.posicion + separacion;
    }
}

void Simulador::manejarColisionPared(Particula& p) {
    bool colision = false;

    // Colisión con pared izquierda
    if (p.posicion.x - p.radio < 0) {
        p.posicion.x = p.radio;
        p.velocidad.x = -p.velocidad.x;
        colision = true;
    }

    // Colisión con pared derecha
    if (p.posicion.x + p.radio > caja.ancho) {
        p.posicion.x = caja.ancho - p.radio;
        p.velocidad.x = -p.velocidad.x;
        colision = true;
    }

    // Colisión con pared inferior
    if (p.posicion.y - p.radio < 0) {
        p.posicion.y = p.radio;
        p.velocidad.y = -p.velocidad.y;
        colision = true;
    }

    // Colisión con pared superior
    if (p.posicion.y + p.radio > caja.alto) {
        p.posicion.y = caja.alto - p.radio;
        p.velocidad.y = -p.velocidad.y;
        colision = true;
    }

    // Registrar colisión si ocurrió
    if (colision) {
        registrarColision("COLISION_PARED", p.getId());
    }
}

// Utilidades privadas
void Simulador::guardarPosiciones() {
    for (const auto& p : particulas) {
        if (p.activa) {
            archivo << fixed << setprecision(4);
            archivo << tiempo << ",POSICION," << p.getId() << ","
                    << p.posicion.x << "," << p.posicion.y << ","
                    << p.radio << "," << p.masa << endl;
        }
    }
}

void Simulador::registrarColision(const string& tipo, int id1, int id2) {
    archivo << fixed << setprecision(4);
    archivo << tiempo << "," << tipo << "," << id1;
    if (id2 >= 0) {
        archivo << "," << id2;
    }
    archivo << endl;
}

// Utilidades públicas
void Simulador::imprimirConfiguracion() const {
    cout << "\n=== CONFIGURACION DE LA SIMULACION ===" << endl;
    caja.imprimirInfo();
    cout << "Timestep (dt): " << dt << " segundos" << endl;
    cout << "Coeficiente de restitucion: " << coefRestitucion << endl;

    cout << "\nParticulas (" << particulas.size() << "):" << endl;
    for (const auto& p : particulas) {
        cout << "  ";
        p.imprimirInfo();
    }

    cout << "\nObstaculos (" << obstaculos.size() << "):" << endl;
    for (const auto& obs : obstaculos) {
        cout << "  ";
        obs.imprimirInfo();
    }
    cout << endl;
}
