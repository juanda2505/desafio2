// clase album
// cada album sabe de quien es (posicion del usuario) y donde estan sus fotos.

#ifndef ALBUM_H
#define ALBUM_H

#include <string>

// privacidad como enum: ocupa menos que un texto y se compara en un paso
enum class Privacidad { PUBLICO, PRIVADO, AMIGOS };

class Album {
private:
    int         idPropietario;   // posicion del dueno en el arreglo de usuarios
    std::string titulo;
    std::string ruta;
    Privacidad  privacidad;
    // las fotos de un album quedan seguidas en el arreglo de fotos,
    // asi que basta con saber donde empiezan y cuantas son
    int         primeraFoto;     // -1 si el album esta vacio
    int         cantidadFotos;

public:
    Album();

    // llena el album con los campos ya separados de una linea de albumes.txt:
    // usuario,album,ruta,privacidad   (el usuario ya viene convertido a posicion)
    bool cargar(int idPropietario, const std::string& titulo,
                const std::string& ruta, const std::string& privacidadTexto);

    // registra que la foto en la posicion 'indice' pertenece a este album.
    // devuelve false si la foto no queda seguida de las anteriores.
    bool agregarFoto(int indice);

    // getters
    int                getIdPropietario() const { return idPropietario; }
    const std::string& getTitulo()        const { return titulo; }
    const std::string& getRuta()          const { return ruta; }
    Privacidad         getPrivacidad()    const { return privacidad; }
    int                getPrimeraFoto()   const { return primeraFoto; }
    int                getCantidadFotos() const { return cantidadFotos; }

    const char* privacidadTexto() const;
};

#endif
