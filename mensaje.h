// clase mensaje
// cada mensaje existe una sola vez (en el arreglo de la red social) y el
// emisor y el receptor guardan su posicion: asi se ve desde los dos lados

#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include "fecha.h"

class Mensaje {
private:
    int         idRemitente;
    int         idDestinatario;
    std::string contenido;
    Fecha       fechaEnvio;
    bool        leido;

public:
    static const int MAX_CARACTERES = 512;

    Mensaje();
    Mensaje(int remitente, int destinatario, const std::string& contenido,
            const Fecha& fecha, bool leido = false);

    // getters
    int                getIdRemitente()    const { return idRemitente; }
    int                getIdDestinatario() const { return idDestinatario; }
    const std::string& getContenido()      const { return contenido; }
    const Fecha&       getFechaEnvio()     const { return fechaEnvio; }
    bool               estaLeido()         const { return leido; }

    void marcarComoLeido() { leido = true; }
    // incluye las dos fechas del rango
    bool esEntreFechas(const Fecha& inicio, const Fecha& fin) const {
        return inicio <= fechaEnvio && fechaEnvio <= fin;
    }

    // bytes en el heap de este mensaje (su texto)
    long long memoria() const;
};

#endif
