// clase usuario
// un atributo por cada campo de usuarios.txt, mas sus amigos
// y las solicitudes de amistad que ha recibido

#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "fecha.h"
#include "arreglo.h"
#include "solicitudamistad.h"

class Usuario {
private:
    int         idUsuario;       // posicion del usuario en el arreglo (no viene en el archivo)
    std::string nombre;          // nombre de usuario unico
    std::string contrasena;
    Fecha       fechaNacimiento;
    std::string correo;
    std::string fotoPerfil;      // ruta de la foto de perfil
    std::string ciudad;
    std::string pais;
    Fecha       fechaInscripcion;

    // amigos: posiciones de los otros usuarios (asociacion usuario-usuario del diagrama)
    Arreglo<int> amigos;
    // solicitudes que otros le enviaron a este usuario
    Arreglo<SolicitudAmistad> solicitudesRecibidas;
    // los albumes de un usuario quedan seguidos en el arreglo de albumes,
    // asi que basta con saber donde empiezan y cuantos son (como las fotos del album)
    int primerAlbum;
    int cantidadAlbumes;

public:
    Usuario();
    // no hace falta escribir constructor de copia ni destructor:
    // arreglo ya copia y libera su propia memoria

    // recibe una linea del archivo y llena los atributos.
    // devuelve false si la linea no tiene todos los campos.
    bool cargarDesdeLinea(const std::string& linea, int id);

    // getters
    int                getIdUsuario()        const { return idUsuario; }
    const std::string& getNombre()           const { return nombre; }
    const std::string& getContrasena()       const { return contrasena; }
    const std::string& getCorreo()           const { return correo; }
    const std::string& getFotoPerfil()       const { return fotoPerfil; }
    const Fecha&       getFechaNacimiento()  const { return fechaNacimiento; }
    const std::string& getCiudad()           const { return ciudad; }
    const std::string& getPais()             const { return pais; }
    const Fecha&       getFechaInscripcion() const { return fechaInscripcion; }

    // albumes
    // registra que el album en la posicion 'indice' es de este usuario.
    // devuelve false si no queda seguido de los anteriores
    bool agregarAlbum(int indice);
    int  getPrimerAlbum()     const { return primerAlbum; }
    int  getCantidadAlbumes() const { return cantidadAlbumes; }

    // amigos
    void agregarAmigo(int idAmigo);
    bool esAmigo(int idOtro) const;
    const Arreglo<int>& obtenerAmigos() const { return amigos; }

    // solicitudes
    // devuelve un codigo: 0 enviada, 1 a si mismo, 2 ya son amigos,
    // 3 ya le habia enviado una, 4 el otro ya le habia enviado una
    int  enviarSolicitud(Usuario& receptor, const Fecha& fecha);
    void recibirSolicitud(const SolicitudAmistad& solicitud);   // tambien lo usa la carga
    bool tieneSolicitudPendienteDe(int idEmisor) const;
    const Arreglo<SolicitudAmistad>& getSolicitudesRecibidas() const { return solicitudesRecibidas; }
    // cambian el estado de la solicitud en la posicion indicada.
    // aceptar devuelve la posicion del emisor (para agregar la amistad) o -1
    int  aceptarSolicitud(int posicion);
    bool rechazarSolicitud(int posicion);

    void mostrar() const;

    // bytes en el heap de este usuario (textos, amigos y solicitudes)
    long long memoria() const;

    // sobrecarga de operadores: dos usuarios son iguales si tienen el mismo nombre
    bool operator==(const Usuario& otro) const;
    bool operator!=(const Usuario& otro) const { return !(*this == otro); }
};

#endif
