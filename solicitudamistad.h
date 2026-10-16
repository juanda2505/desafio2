// clase solicitud de amistad
// guarda quien la envia, quien la recibe, cuando y en que estado esta.
// los usuarios se guardan por su posicion en el arreglo, no por nombre

#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H

#include <string>
#include "fecha.h"

enum class EstadoSolicitud { PENDIENTE, ACEPTADA, RECHAZADA };

class SolicitudAmistad {
private:
    int             idEmisor;
    int             idReceptor;
    Fecha           fechaCreacion;
    EstadoSolicitud estado;

public:
    SolicitudAmistad();
    SolicitudAmistad(int emisor, int receptor, const Fecha& fecha,
                     EstadoSolicitud estado = EstadoSolicitud::PENDIENTE);

    // solo cambian el estado si la solicitud sigue pendiente
    bool aceptar();
    bool rechazar();

    // getters
    int             getIdEmisor()      const { return idEmisor; }
    int             getIdReceptor()    const { return idReceptor; }
    const Fecha&    getFechaCreacion() const { return fechaCreacion; }
    EstadoSolicitud obtenerEstado()    const { return estado; }
    bool            estaPendiente()    const { return estado == EstadoSolicitud::PENDIENTE; }

    // conversion del estado a texto y de texto a estado (para el archivo)
    const char* estadoTexto() const;
    static bool estadoDesdeTexto(const std::string& texto, EstadoSolicitud& estado);
};

#endif
