// clase foto
// cada foto sabe a que album pertenece (posicion del album en el arreglo).

#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "fecha.h"
#include "arreglo.h"

class Foto {
private:
    int         idAlbum;        // posicion del album en el arreglo de albumes
    std::string nombre;
    Fecha       fechaCaptura;
    int         tamanoKB;
    std::string ruta;
    int         likesIniciales; // likes que trae fotos.txt (no se sabe quien los dio)
    // usuarios que dieron like dentro de la plataforma. guardar quien lo dio
    // permite quitar el like de forma correcta y evitar que alguien lo repita
    Arreglo<int> usuariosLike;

public:
    Foto();

    // llena la foto con los campos ya separados de una linea de fotos.txt:
    // usuario,album,foto,fecha_captura,tamano_kb,ruta,likes
    bool cargar(int idAlbum, const std::string& nombre, const std::string& fecha,
                const std::string& tamano, const std::string& ruta, const std::string& likes);

    // getters
    int                getIdAlbum()      const { return idAlbum; }
    const std::string& getNombre()       const { return nombre; }
    const Fecha&       getFechaCaptura() const { return fechaCaptura; }
    int                getTamanoKB()     const { return tamanoKB; }
    const std::string& getRuta()         const { return ruta; }
    int                getLikes()        const { return likesIniciales + usuariosLike.tamano(); }
    const Arreglo<int>& getUsuariosLike() const { return usuariosLike; }

    // likes desde el carrusel
    bool tieneLikeDe(int idUsuario) const { return usuariosLike.buscar(idUsuario) != -1; }
    // da el like si no lo tenia o lo quita si ya lo tenia; devuelve true si quedo con like
    bool alternarLike(int idUsuario);
    void agregarLike(int idUsuario);   // lo usa la carga de likes.txt

    // bytes en el heap de esta foto (textos y lista de likes)
    long long memoria() const;
};

#endif
