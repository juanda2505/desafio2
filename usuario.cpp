// implementacion de la clase usuario

#include "usuario.h"
#include "archivo.h"
#include <iostream>

Usuario::Usuario() : idUsuario(-1) {}

// lectura y separacion de campos
bool Usuario::cargarDesdeLinea(const std::string& linea, int id) {
    // orden de los campos en el archivo, separados por ',':
    // usuario,fecha_nacimiento,correo,foto_perfil,contrasena,ciudad,pais,fecha_inscripcion
    const int CAMPOS = 8;
    std::string campo[CAMPOS];
    if (!Archivo::separarCampos(linea, campo, CAMPOS)) return false;

    // se guardan los campos en el objeto
    idUsuario        = id;
    nombre           = campo[0];
    fechaNacimiento  = Fecha::desdeTexto(campo[1]);
    correo           = campo[2];
    fotoPerfil       = campo[3];
    contrasena       = campo[4];
    ciudad           = campo[5];
    pais             = campo[6];
    fechaInscripcion = Fecha::desdeTexto(campo[7]);
    return true;
}

// amigos

void Usuario::agregarAmigo(int idAmigo) {
    if (idAmigo != idUsuario && !esAmigo(idAmigo)) amigos.agregar(idAmigo);
}

bool Usuario::esAmigo(int idOtro) const {
    return amigos.buscar(idOtro) != -1;
}

// solicitudes

// las verificaciones van en el orden del enunciado; cada una tiene su codigo
// para que el menu muestre un mensaje distinto
int Usuario::enviarSolicitud(Usuario& receptor, const Fecha& fecha) {
    if (receptor.idUsuario == idUsuario)            return 1;
    if (esAmigo(receptor.idUsuario))                return 2;
    if (receptor.tieneSolicitudPendienteDe(idUsuario)) return 3;
    if (tieneSolicitudPendienteDe(receptor.idUsuario)) return 4;
    receptor.recibirSolicitud(SolicitudAmistad(idUsuario, receptor.idUsuario, fecha));
    return 0;
}

void Usuario::recibirSolicitud(const SolicitudAmistad& solicitud) {
    solicitudesRecibidas.agregar(solicitud);
}

bool Usuario::tieneSolicitudPendienteDe(int idEmisor) const {
    for (int i = 0; i < solicitudesRecibidas.tamano(); i++) {
        const SolicitudAmistad& s = solicitudesRecibidas[i];
        if (s.estaPendiente() && s.getIdEmisor() == idEmisor) return true;
    }
    return false;
}

int Usuario::aceptarSolicitud(int posicion) {
    if (posicion < 0 || posicion >= solicitudesRecibidas.tamano()) return -1;
    if (!solicitudesRecibidas[posicion].aceptar()) return -1;
    return solicitudesRecibidas[posicion].getIdEmisor();
}

bool Usuario::rechazarSolicitud(int posicion) {
    if (posicion < 0 || posicion >= solicitudesRecibidas.tamano()) return false;
    return solicitudesRecibidas[posicion].rechazar();
}

// muestra los datos del usuario (las fechas sin hora)
void Usuario::mostrar() const {
    std::cout << "[" << idUsuario << "] " << nombre
              << " | " << correo
              << " | foto: " << fotoPerfil
              << " | nacimiento: " << fechaNacimiento.aTextoFecha()
              << " | " << ciudad << ", " << pais
              << " | inscripcion: " << fechaInscripcion.aTextoFecha() << "\n";
}

bool Usuario::operator==(const Usuario& otro) const {
    return Archivo::compararCadenas(nombre, otro.nombre);
}
