// clase fecha
// guarda una fecha con hora y minuto. la hora se usara en los mensajes
// que se ordenan por fecha y hora de envio.

#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <string>

class Fecha {
private:
    short anio;
    char  mes, dia, hora, minuto;   // 1 byte cada uno: los valores son pequenos

    long long clave() const;        // aaaammddhhmm en un solo numero, para comparar rapido

public:
    Fecha();
    Fecha(int anio, int mes, int dia, int hora = 0, int minuto = 0);
    Fecha(const Fecha& otra);                 // constructor de copia
    Fecha& operator=(const Fecha& otra);

    // convierte "aaaa-mm-dd" o "aaaa-mm-dd hh:mm" en fecha
    static Fecha desdeTexto(const std::string& texto);
    // fecha y hora actual de colombia (utc-5), para las solicitudes y mensajes
    static Fecha ahora();
    std::string aTexto() const;        // "aaaa-mm-dd hh:mm" (mensajes)
    std::string aTextoFecha() const;   // "aaaa-mm-dd" (nacimiento, inscripcion)
    bool esValida() const;

    int getAnio()   const { return anio; }
    int getMes()    const { return mes; }
    int getDia()    const { return dia; }
    int getHora()   const { return hora; }
    int getMinuto() const { return minuto; }

    // sobrecarga de operadores de comparacion
    bool operator==(const Fecha& o) const;
    bool operator!=(const Fecha& o) const;
    bool operator< (const Fecha& o) const;
    bool operator<=(const Fecha& o) const;
    bool operator> (const Fecha& o) const;
    bool operator>=(const Fecha& o) const;

    // sobrecarga de << como funcion amiga, para imprimir con cout
    friend std::ostream& operator<<(std::ostream& os, const Fecha& f);
};

#endif
