// clase central de la plataforma
// es duena de todos los usuarios, albumes y fotos; los carga desde los archivos,
// guarda los cambios al salir y maneja el menu por consola

#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include "usuario.h"
#include "album.h"
#include "foto.h"

class RedSocial {
private:
    std::string carpeta;          // carpeta donde estan los archivos de datos

    Usuario* usuarios;
    int      cantidadUsuarios;
    int*     ordenPorNombre;      // posiciones de los usuarios ordenadas por nombre

    Album*   albumes;
    int      cantidadAlbumes;
    Foto*    fotos;
    int      cantidadFotos;

    // solo se reescriben los archivos si algo cambio durante la ejecucion
    bool     cambiosAmistades;
    bool     cambiosSolicitudes;

    // carga desde los archivos
    bool cargarUsuarios();
    void construirIndiceNombres();
    bool cargarAlbumes();
    bool cargarFotos();
    bool cargarAmistades();
    void cargarSolicitudes();     // el archivo es opcional: al inicio no existe

    // guardado en los archivos; devuelven false si no se pudo escribir
    bool guardarAmistades() const;
    bool guardarSolicitudes() const;

    // busquedas
    int buscarAlbum(int idUsuario, const std::string& titulo, int& pista) const;

    // menu
    // valor que devuelve leerOpcion cuando ya no hay mas entrada
    static const int FIN_ENTRADA = -2;
    static int leerOpcion();
    int  iniciarSesion();
    void menuPrincipal(int idActual);
    void menuAmistades(int idActual);
    void enviarSolicitud(int idActual);
    void verSolicitudesRecibidas(int idActual);

public:
    explicit RedSocial(const std::string& carpetaDatos);
    ~RedSocial();

    // la red social es duena unica de sus arreglos: no se permite copiarla,
    // porque dos copias liberarian la misma memoria
    RedSocial(const RedSocial&) = delete;
    RedSocial& operator=(const RedSocial&) = delete;

    bool cargarDatos();
    // guarda solo lo que cambio y vuelve a poner los indicadores en falso,
    // para no reescribir lo mismo dos veces. se llama al cerrar sesion y al salir
    void guardarDatos();
    void ejecutar();

    // busqueda binaria sobre el indice ordenado; devuelve la posicion o -1
    int buscarUsuario(const std::string& nombre) const;
};

#endif
