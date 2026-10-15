#include "foto.h"
#include "utilidades.h"

Foto::Foto() : idAlbum(-1), tamanoKB(0), likes(0) {}

bool Foto::cargar(int idAlb, const std::string& n, const std::string& fecha,
                  const std::string& tamano, const std::string& r, const std::string& lk) {
    int t, l;
    if (idAlb < 0 || n.empty())       return false;
    if (!textoAEntero(tamano, t))     return false;
    if (!textoAEntero(lk, l))         return false;
    Fecha f = Fecha::desdeTexto(fecha);
    if (!f.esValida())                return false;

    idAlbum      = idAlb;
    nombre       = n;
    fechaCaptura = f;
    tamanoKB     = t;
    ruta         = r;
    likes        = l;
    return true;
}
