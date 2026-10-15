#include "album.h"
#include "usuario.h"   // compararCadenas

Album::Album()
    : idPropietario(-1), privacidad(Privacidad::PRIVADO), primeraFoto(-1), cantidadFotos(0) {}

bool Album::cargar(int idDueno, const std::string& t, const std::string& r,
                   const std::string& privTexto) {
    if (idDueno < 0 || t.empty()) return false;

    if      (compararCadenas(privTexto, "publico")) privacidad = Privacidad::PUBLICO;
    else if (compararCadenas(privTexto, "privado")) privacidad = Privacidad::PRIVADO;
    else if (compararCadenas(privTexto, "amigos"))  privacidad = Privacidad::AMIGOS;
    else return false;                         // privacidad desconocida

    idPropietario = idDueno;
    titulo        = t;
    ruta          = r;
    primeraFoto   = -1;
    cantidadFotos = 0;
    return true;
}

bool Album::agregarFoto(int indice) {
    if (cantidadFotos == 0) {
        primeraFoto = indice;
    } else if (indice != primeraFoto + cantidadFotos) {
        return false;                          // la foto no es contigua
    }
    cantidadFotos++;
    return true;
}

const char* Album::privacidadTexto() const {
    switch (privacidad) {
        case Privacidad::PUBLICO: return "publico";
        case Privacidad::AMIGOS:  return "amigos";
        default:                  return "privado";
    }
}
