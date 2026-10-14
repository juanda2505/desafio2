#include "fecha.h"
#include <cstdio>

Fecha::Fecha() : anio(2000), mes(1), dia(1), hora(0), minuto(0) {}

Fecha::Fecha(int a, int m, int d, int h, int mi)
    : anio(a), mes(m), dia(d), hora(h), minuto(mi) {}

Fecha::Fecha(const Fecha& o)
    : anio(o.anio), mes(o.mes), dia(o.dia), hora(o.hora), minuto(o.minuto) {}

Fecha& Fecha::operator=(const Fecha& o) {
    anio = o.anio; mes = o.mes; dia = o.dia; hora = o.hora; minuto = o.minuto;
    return *this;
}

long long Fecha::clave() const {
    return (((((long long)anio * 100 + mes) * 100 + dia) * 100 + hora) * 100) + minuto;
}

// Convierte los dígitos de texto[ini..ini+largo) en entero; -1 si no son dígitos.
static int leerNumero(const std::string& t, int ini, int largo) {
    if (ini + largo > (int)t.size()) return -1;
    int n = 0;
    for (int i = ini; i < ini + largo; i++) {
        if (t[i] < '0' || t[i] > '9') return -1;
        n = n * 10 + (t[i] - '0');
    }
    return n;
}

Fecha Fecha::desdeTexto(const std::string& t) {
    int a = leerNumero(t, 0, 4), m = leerNumero(t, 5, 2), d = leerNumero(t, 8, 2);
    int h = 0, mi = 0;
    if (t.size() >= 16) { h = leerNumero(t, 11, 2); mi = leerNumero(t, 14, 2); }
    if (a < 0 || m < 0 || d < 0 || h < 0 || mi < 0) return Fecha(0, 0, 0);  // inválida
    return Fecha(a, m, d, h, mi);
}

bool Fecha::esValida() const {
    if (mes < 1 || mes > 12 || dia < 1 || hora < 0 || hora > 23 || minuto < 0 || minuto > 59)
        return false;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
    int max = diasMes[mes - 1] + ((mes == 2 && bisiesto) ? 1 : 0);
    return dia <= max;
}

std::string Fecha::aTexto() const {
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d", anio, mes, dia, hora, minuto);
    return std::string(buf);
}

bool Fecha::operator==(const Fecha& o) const { return clave() == o.clave(); }
bool Fecha::operator!=(const Fecha& o) const { return clave() != o.clave(); }
bool Fecha::operator< (const Fecha& o) const { return clave() <  o.clave(); }
bool Fecha::operator<=(const Fecha& o) const { return clave() <= o.clave(); }
bool Fecha::operator> (const Fecha& o) const { return clave() >  o.clave(); }
bool Fecha::operator>=(const Fecha& o) const { return clave() >= o.clave(); }

std::ostream& operator<<(std::ostream& os, const Fecha& f) {
    os << f.aTexto();
    return os;
}