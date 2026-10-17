// plantilla de arreglo dinamico propio (reemplaza a vector, que no se permite)
// crece al doble cuando se llena: agregar al final cuesta o(1) en promedio
// y el acceso por posicion es directo

#ifndef ARREGLO_H
#define ARREGLO_H

#include "metricas.h"

template <typename T>
class Arreglo {
private:
    T*  datos;
    int cantidad;
    int capacidad;

    // reserva un arreglo del doble de tamano y copia los elementos
    void crecer() {
        int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) { Metricas::contar(); nuevo[i] = datos[i]; }
        delete[] datos;
        datos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    Arreglo() : datos(nullptr), cantidad(0), capacidad(0) {}

    // constructor de copia: copia profunda, cada lista tiene su propio arreglo
    Arreglo(const Arreglo& otra)
        : datos(nullptr), cantidad(otra.cantidad), capacidad(otra.capacidad) {
        if (capacidad > 0) {
            datos = new T[capacidad];
            for (int i = 0; i < cantidad; i++) datos[i] = otra.datos[i];
        }
    }

    // asignacion con la misma idea que el constructor de copia
    Arreglo& operator=(const Arreglo& otra) {
        if (this != &otra) {
            delete[] datos;
            cantidad = otra.cantidad;
            capacidad = otra.capacidad;
            datos = (capacidad > 0) ? new T[capacidad] : nullptr;
            for (int i = 0; i < cantidad; i++) datos[i] = otra.datos[i];
        }
        return *this;
    }

    ~Arreglo() { delete[] datos; }

    // reserva de una vez espacio para 'n' elementos (cuando ya se sabe cuantos vienen)
    void reservar(int n) {
        if (n <= capacidad) return;
        T* nuevo = new T[n];
        for (int i = 0; i < cantidad; i++) { Metricas::contar(); nuevo[i] = datos[i]; }
        delete[] datos;
        datos = nuevo;
        capacidad = n;
    }

    void agregar(const T& valor) {
        if (cantidad == capacidad) crecer();
        datos[cantidad++] = valor;
    }

    // inserta en la posicion indicada corriendo los siguientes a la derecha
    void insertar(int posicion, const T& valor) {
        if (posicion < 0) posicion = 0;
        if (posicion > cantidad) posicion = cantidad;
        if (cantidad == capacidad) crecer();
        for (int i = cantidad; i > posicion; i--) { Metricas::contar(); datos[i] = datos[i - 1]; }
        datos[posicion] = valor;
        cantidad++;
    }

    // quita la posicion indicada corriendo los siguientes a la izquierda
    bool eliminar(int posicion) {
        if (posicion < 0 || posicion >= cantidad) return false;
        for (int i = posicion; i < cantidad - 1; i++) { Metricas::contar(); datos[i] = datos[i + 1]; }
        cantidad--;
        return true;
    }

    // busqueda lineal; devuelve la posicion o -1
    int buscar(const T& valor) const {
        for (int i = 0; i < cantidad; i++) {
            Metricas::contar();
            if (datos[i] == valor) return i;
        }
        return -1;
    }

    T&       operator[](int i)       { return datos[i]; }
    const T& operator[](int i) const { return datos[i]; }

    int  tamano() const { return cantidad; }
    bool vacia()  const { return cantidad == 0; }

    // bytes reservados en el heap por el arreglo (sin contar lo que apunten los elementos)
    long long memoria() const { return (long long)sizeof(T) * capacidad; }
};

#endif
