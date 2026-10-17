// implementacion de la clase album

#include "album.h"
#include "archivo.h"
#include "metricas.h"

Album::Album()
    : idPropietario(-1), privacidad(Privacidad::PRIVADO), primeraFoto(-1), cantidadFotos(0) {}

// valida los datos y convierte el texto de privacidad al enum
bool Album::cargar(int idDueno, const std::string& t, const std::string& r,
                   const std::string& privTexto) {
    if (idDueno < 0 || t.empty()) return false;

    if      (Archivo::compararCadenas(privTexto, "publico")) privacidad = Privacidad::PUBLICO;
    else if (Archivo::compararCadenas(privTexto, "privado")) privacidad = Privacidad::PRIVADO;
    else if (Archivo::compararCadenas(privTexto, "amigos"))  privacidad = Privacidad::AMIGOS;
    else return false;                         // privacidad desconocida

    idPropietario = idDueno;
    titulo        = t;
    ruta          = r;
    primeraFoto   = -1;
    cantidadFotos = 0;
    return true;
}

// la primera foto marca el inicio; las siguientes deben venir seguidas
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

bool Album::esVisiblePara(int idVisitante, bool esAmigoDelDueno) const {
    if (idVisitante == idPropietario)       return true;
    if (privacidad == Privacidad::PUBLICO)  return true;
    if (privacidad == Privacidad::AMIGOS)   return esAmigoDelDueno;
    return false;                           // privado
}

long long Album::memoria() const {
    return Metricas::memoriaTexto(titulo) + Metricas::memoriaTexto(ruta);
}
