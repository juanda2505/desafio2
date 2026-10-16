// implementacion de la clase fecha

#include "fecha.h"
#include <chrono>

// constructores y asignacion
Fecha::Fecha() : anio(2000), mes(1), dia(1), hora(0), minuto(0) {}

Fecha::Fecha(int a, int m, int d, int h, int mi)
    : anio(a), mes(m), dia(d), hora(h), minuto(mi) {}

Fecha::Fecha(const Fecha& o)
    : anio(o.anio), mes(o.mes), dia(o.dia), hora(o.hora), minuto(o.minuto) {}

Fecha& Fecha::operator=(const Fecha& o) {
    anio = o.anio; mes = o.mes; dia = o.dia; hora = o.hora; minuto = o.minuto;
    return *this;
}

// junta toda la fecha en un solo numero: comparar dos fechas es una sola operacion
long long Fecha::clave() const {
    return (((((long long)anio * 100 + mes) * 100 + dia) * 100 + hora) * 100) + minuto;
}

// lectura desde texto

// convierte los digitos de t[ini .. ini+largo) en entero; -1 si alguno no es digito
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
    if (a < 0 || m < 0 || d < 0 || h < 0 || mi < 0) return Fecha(0, 0, 0);  // fecha invalida
    return Fecha(a, m, d, h, mi);
}

// toma los segundos del reloj del sistema y los convierte a fecha.
// el paso de dias a anio-mes-dia es el algoritmo de calendario civil
// (dias contados desde 1970-01-01)
Fecha Fecha::ahora() {
    long long segundos = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    segundos -= 5 * 3600;                         // colombia: utc-5
    long long dias = segundos / 86400;
    int segDelDia  = (int)(segundos % 86400);

    dias += 719468;                               // se corre el origen al 0000-03-01
    long long era = dias / 146097;                // bloques de 400 anios
    long long diaEra = dias - era * 146097;
    long long anioEra = (diaEra - diaEra / 1460 + diaEra / 36524 - diaEra / 146096) / 365;
    long long a = anioEra + era * 400;
    long long diaAnio = diaEra - (365 * anioEra + anioEra / 4 - anioEra / 100);
    long long mp = (5 * diaAnio + 2) / 153;       // mes contando desde marzo
    int d = (int)(diaAnio - (153 * mp + 2) / 5 + 1);
    int m = (int)(mp < 10 ? mp + 3 : mp - 9);
    if (m <= 2) a++;

    return Fecha((int)a, m, d, segDelDia / 3600, (segDelDia % 3600) / 60);
}

// revisa rangos de mes, dia (con bisiestos), hora y minuto
bool Fecha::esValida() const {
    if (mes < 1 || mes > 12 || dia < 1 || hora < 0 || hora > 23 || minuto < 0 || minuto > 59)
        return false;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
    int max = diasMes[mes - 1] + ((mes == 2 && bisiesto) ? 1 : 0);
    return dia <= max;
}

// conversion a texto (ciclo propio, sin snprintf)

// agrega n al texto con al menos 'ancho' digitos, rellenando con ceros a la izquierda
static void agregarNumero(std::string& texto, int n, int ancho) {
    char digitos[10];
    int k = 0;
    do {
        digitos[k++] = '0' + (n % 10);
        n /= 10;
    } while (n > 0 && k < 10);
    while (k < ancho) digitos[k++] = '0';
    while (k > 0) texto += digitos[--k];   // los digitos se sacaron al reves
}

std::string Fecha::aTextoFecha() const {
    std::string t;
    agregarNumero(t, anio, 4); t += '-';
    agregarNumero(t, mes, 2);  t += '-';
    agregarNumero(t, dia, 2);
    return t;
}

std::string Fecha::aTexto() const {
    std::string t = aTextoFecha();
    t += ' ';
    agregarNumero(t, hora, 2);   t += ':';
    agregarNumero(t, minuto, 2);
    return t;
}

// sobrecarga de operadores
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
