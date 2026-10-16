// implementacion de la clase archivo

#include "archivo.h"
#include <fstream>

// recorre la linea; cada coma cierra un campo y pasa al siguiente
bool Archivo::separarCampos(const std::string& linea, std::string campos[], int esperados) {
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
    return n == esperados - 1;                 // si es menor, faltan campos
}

// arma el numero digito por digito
bool Archivo::textoAEntero(const std::string& texto, int& resultado) {
    if (texto.empty()) return false;
    int valor = 0;
    for (unsigned i = 0; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
        valor = valor * 10 + (texto[i] - '0');
    }
    resultado = valor;
    return true;
}

// primera pasada sobre el archivo: solo cuenta, para reservar el tamano exacto
int Archivo::contarLineasDeDatos(const std::string& ruta) {
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

bool Archivo::compararCadenas(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;    // distinto largo: no hace falta recorrer
    for (unsigned i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// compara caracter por caracter; el primero distinto decide.
// si uno es prefijo del otro, el mas corto va primero
bool Archivo::esMenor(const std::string& a, const std::string& b) {
    unsigned n = (a.size() < b.size()) ? a.size() : b.size();
    for (unsigned i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return a.size() < b.size();
}
