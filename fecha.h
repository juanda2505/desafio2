#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <string>

// Fecha con hora y minuto (los mensajes se ordenan por fecha de envío,
// así que solo el día no alcanza para desempatar).
class Fecha {
private:
    short anio;
    char  mes, dia, hora, minuto;   // 1 byte cada uno: valores pequeños

    long long clave() const;        // AAAAMMDDhhmm, para comparar rápido

public:
    Fecha();
    Fecha(int anio, int mes, int dia, int hora = 0, int minuto = 0);
    Fecha(const Fecha& otra);
    Fecha& operator=(const Fecha& otra);

    // Acepta "AAAA-MM-DD" o "AAAA-MM-DD hh:mm"
    static Fecha desdeTexto(const std::string& texto);
    std::string aTexto() const;
    bool esValida() const;

    int getAnio()   const { return anio; }
    int getMes()    const { return mes; }
    int getDia()    const { return dia; }
    int getHora()   const { return hora; }
    int getMinuto() const { return minuto; }

    // Sobrecarga de operadores
    bool operator==(const Fecha& o) const;
    bool operator!=(const Fecha& o) const;
    bool operator< (const Fecha& o) const;
    bool operator<=(const Fecha& o) const;
    bool operator> (const Fecha& o) const;
    bool operator>=(const Fecha& o) const;

    friend std::ostream& operator<<(std::ostream& os, const Fecha& f);
};

#endif