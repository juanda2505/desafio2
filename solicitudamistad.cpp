// implementacion de la clase solicitud de amistad

#include "solicitudamistad.h"
#include "archivo.h"

SolicitudAmistad::SolicitudAmistad()
    : idEmisor(-1), idReceptor(-1), estado(EstadoSolicitud::PENDIENTE) {}

SolicitudAmistad::SolicitudAmistad(int emisor, int receptor, const Fecha& fecha, EstadoSolicitud e)
    : idEmisor(emisor), idReceptor(receptor), fechaCreacion(fecha), estado(e) {}

bool SolicitudAmistad::aceptar() {
    if (estado != EstadoSolicitud::PENDIENTE) return false;
    estado = EstadoSolicitud::ACEPTADA;
    return true;
}

bool SolicitudAmistad::rechazar() {
    if (estado != EstadoSolicitud::PENDIENTE) return false;
    estado = EstadoSolicitud::RECHAZADA;
    return true;
}

const char* SolicitudAmistad::estadoTexto() const {
    switch (estado) {
        case EstadoSolicitud::ACEPTADA:  return "aceptada";
        case EstadoSolicitud::RECHAZADA: return "rechazada";
        default:                         return "pendiente";
    }
}

bool SolicitudAmistad::estadoDesdeTexto(const std::string& t, EstadoSolicitud& e) {
    if      (Archivo::compararCadenas(t, "pendiente")) e = EstadoSolicitud::PENDIENTE;
    else if (Archivo::compararCadenas(t, "aceptada"))  e = EstadoSolicitud::ACEPTADA;
    else if (Archivo::compararCadenas(t, "rechazada")) e = EstadoSolicitud::RECHAZADA;
    else return false;
    return true;
}
