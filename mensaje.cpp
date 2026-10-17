// implementacion de la clase mensaje

#include "mensaje.h"
#include "metricas.h"

Mensaje::Mensaje() : idRemitente(-1), idDestinatario(-1), leido(false) {}

Mensaje::Mensaje(int remitente, int destinatario, const std::string& texto,
                 const Fecha& fecha, bool l)
    : idRemitente(remitente), idDestinatario(destinatario), contenido(texto),
      fechaEnvio(fecha), leido(l) {}

long long Mensaje::memoria() const {
    return Metricas::memoriaTexto(contenido);
}
