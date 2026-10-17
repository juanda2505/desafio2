// implementacion de la clase foto

#include "foto.h"
#include "archivo.h"

Foto::Foto() : idAlbum(-1), tamanoKB(0), likesIniciales(0) {}

// valida numeros y fecha antes de guardar; si algo falla la foto se descarta
bool Foto::cargar(int idAlb, const std::string& n, const std::string& fecha,
                  const std::string& tamano, const std::string& r, const std::string& lk) {
    int t, l;
    if (idAlb < 0 || n.empty())       return false;
    if (!Archivo::textoAEntero(tamano, t))     return false;
    if (!Archivo::textoAEntero(lk, l))         return false;
    Fecha f = Fecha::desdeTexto(fecha);
    if (!f.esValida())                return false;

    idAlbum      = idAlb;
    nombre       = n;
    fechaCaptura = f;
    tamanoKB     = t;
    ruta         = r;
    likesIniciales = l;
    return true;
}

bool Foto::alternarLike(int idUsuario) {
    int posicion = usuariosLike.buscar(idUsuario);
    if (posicion == -1) {
        usuariosLike.agregar(idUsuario);
        return true;
    }
    usuariosLike.eliminar(posicion);
    return false;
}

void Foto::agregarLike(int idUsuario) {
    if (!tieneLikeDe(idUsuario)) usuariosLike.agregar(idUsuario);
}

long long Foto::memoria() const {
    return Metricas::memoriaTexto(nombre) + Metricas::memoriaTexto(ruta) + usuariosLike.memoria();
}
