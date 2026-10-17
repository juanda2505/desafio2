// clase central de la plataforma
// es duena de todos los usuarios, albumes y fotos; los carga desde los archivos,
// guarda los cambios al salir y maneja el menu por consola

#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include <random>
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
    int      capacidadAlbumes;    // tamano reservado (para medir la memoria)
    Foto*    fotos;
    int      cantidadFotos;
    int      capacidadFotos;

    // solo se reescriben los archivos si algo cambio durante la ejecucion
    bool     cambiosAmistades;
    bool     cambiosSolicitudes;
    bool     cambiosLikes;

    // generador de numeros aleatorios (la libreria random esta permitida).
    // se inicia con una semilla distinta en cada ejecucion: no hay patron fijo
    std::mt19937 generador;

    // carga desde los archivos
    bool cargarUsuarios();
    void construirIndiceNombres();
    bool cargarAlbumes();
    bool cargarFotos();
    bool cargarAmistades();
    void cargarSolicitudes();     // el archivo es opcional: al inicio no existe
    void cargarLikes();           // tambien opcional

    // guardado en los archivos; devuelven false si no se pudo escribir
    bool guardarAmistades() const;
    bool guardarSolicitudes() const;
    bool guardarLikes() const;

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
    void visitarPerfil(int idActual);

    // fotos de perfil de amigos al iniciar sesion
    static int pesoPorComunes(int comunes);
    void contarAmigosEnComun(int idActual, Arreglo<int>& comunes);
    void elegirPonderado(const Arreglo<int>& pesos, bool* bloqueado, int cantidad, Arreglo<int>& elegidos);
    void mostrarFotosAmigos(int idActual, const Arreglo<int>& comunes, Arreglo<int>& anteriores,
                            const char* funcionalidad);
    void carrusel(int idActual, int idAlbum);
    void mostrarFoto(int idActual, int idFoto, int numero, int total) const;

    // memoria de todas las estructuras y objetos del sistema en este momento
    long long memoriaTotal() const;

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
