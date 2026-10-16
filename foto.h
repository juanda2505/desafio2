// clase foto
// cada foto sabe a que album pertenece (posicion del album en el arreglo).

#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "fecha.h"

class Foto {
private:
    int         idAlbum;        // posicion del album en el arreglo de albumes
    std::string nombre;
    Fecha       fechaCaptura;
    int         tamanoKB;
    std::string ruta;
    int         likes;          // contador: no se valida un like por usuario

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
    int                getLikes()        const { return likes; }

    // dar o quitar like desde el carrusel
    void darLike()    { likes++; }
    void quitarLike() { if (likes > 0) likes--; }
};

#endif
