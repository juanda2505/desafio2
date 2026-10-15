#ifndef ALBUM_H
#define ALBUM_H

#include <string>

enum class Privacidad { PUBLICO, PRIVADO, AMIGOS };

class Album {
private:
    int         idPropietario;   // posición del dueño en el arreglo de usuarios
    std::string titulo;
    std::string ruta;
    Privacidad  privacidad;
    // Las fotos de un álbum quedan seguidas en el arreglo de fotos,
    // así que basta con saber dónde empiezan y cuántas son.
    int         primeraFoto;     // -1 si el álbum está vacío
    int         cantidadFotos;

public:
    Album();

    // Llena el álbum con los campos ya separados de una línea de albumes.txt:
    // usuario,album,ruta,privacidad   (el usuario ya viene convertido a id)
    bool cargar(int idPropietario, const std::string& titulo,
                const std::string& ruta, const std::string& privacidadTexto);

    // Registra que la foto en la posición 'indice' pertenece a este álbum.
    // Devuelve false si la foto no queda seguida de las anteriores.
    bool agregarFoto(int indice);

    int                getIdPropietario() const { return idPropietario; }
    const std::string& getTitulo()        const { return titulo; }
    const std::string& getRuta()          const { return ruta; }
    Privacidad         getPrivacidad()    const { return privacidad; }
    int                getPrimeraFoto()   const { return primeraFoto; }
    int                getCantidadFotos() const { return cantidadFotos; }

    const char* privacidadTexto() const;
};

#endif
