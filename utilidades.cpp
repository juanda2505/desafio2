#include "utilidades.h"
#include <fstream>

bool separarCampos(const std::string& linea, std::string campos[], int esperados) {
    for (int k = 0; k < esperados; k++) campos[k].clear();
    int n = 0;
    for (unsigned i = 0; i < linea.size(); i++) {
        char c = linea[i];
        if (c == '\r') continue;
        if (c == ',') {
            n++;
            if (n >= esperados) return false;   // sobran campos
        } else {
            campos[n] += c;
        }
    }
    return n == esperados - 1;                 // faltan campos si es menor
}

bool textoAEntero(const std::string& texto, int& resultado) {
    if (texto.empty()) return false;
    int valor = 0;
    for (unsigned i = 0; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
        valor = valor * 10 + (texto[i] - '0');
    }
    resultado = valor;
    return true;
}

int contarLineasDeDatos(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return -1;
    std::string linea;
    int total = 0;
    std::getline(archivo, linea);              // encabezado
    while (std::getline(archivo, linea)) {
        if (!linea.empty() && linea != "\r") total++;
    }
    return total;
}
