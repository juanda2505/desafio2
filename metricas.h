// clase metricas: medicion del consumo de recursos
// lleva un contador global de iteraciones (cada ciclo del sistema suma uno)
// y ayuda a calcular la memoria de los textos. todo es estatico: no se crean objetos

#ifndef METRICAS_H
#define METRICAS_H

#include <string>

class Metricas {
private:
    static long long iteraciones;

public:
    // se llama al empezar cada funcionalidad
    static void reiniciar() { iteraciones = 0; }
    // se llama en cada vuelta de cada ciclo
    static void contar(long long n = 1) { iteraciones += n; }
    static long long getIteraciones() { return iteraciones; }

    // bytes que un string ocupa en el heap. los textos cortos (hasta 15 caracteres)
    // se guardan dentro del mismo objeto string y no reservan memoria aparte
    static long long memoriaTexto(const std::string& texto) {
        return (texto.capacity() > 15) ? (long long)texto.capacity() + 1 : 0;
    }

    // imprime el reporte al final de una funcionalidad
    static void mostrar(const char* funcionalidad, long long memoriaTotal);
};

#endif
