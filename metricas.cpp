// implementacion de la clase metricas

#include "metricas.h"
#include <iostream>

long long Metricas::iteraciones = 0;

void Metricas::mostrar(const char* funcionalidad, long long memoriaTotal) {
    std::cout << "\n[recursos] " << funcionalidad
              << " | iteraciones: " << iteraciones
              << " | memoria en uso: " << memoriaTotal << " bytes ("
              << memoriaTotal / 1024 << " KB)\n";
}
