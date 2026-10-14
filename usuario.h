#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "fecha.h"

class Usuario {
private:
    int         idUsuario;
    std::string nombre;          // nombre de usuario único
    std::string contrasena;
    Fecha       fechaNacimiento;
    std::string correo;
    std::string fotoPerfil;      // ruta de la foto de perfil
    std::string ciudad;
    std::string pais;
    Fecha       fechaInscripcion;

public:
    Usuario();

    // Recibe una línea del archivo y llena los atributos.
    // Devuelve false si la línea no tiene todos los campos.
    bool cargarDesdeLinea(const std::string& linea, int id);

    int                getIdUsuario()        const { return idUsuario; }
    const std::string& getNombre()           const { return nombre; }
    const std::string& getContrasena()       const { return contrasena; }
    const std::string& getCorreo()           const { return correo; }
    const std::string& getFotoPerfil()       const { return fotoPerfil; }
    const Fecha&       getFechaNacimiento()  const { return fechaNacimiento; }
    const std::string& getCiudad()           const { return ciudad; }
    const std::string& getPais()             const { return pais; }
    const Fecha&       getFechaInscripcion() const { return fechaInscripcion; }

    void mostrar() const;
};

// Compara dos cadenas carácter por carácter con un ciclo propio.
bool compararCadenas(const std::string& a, const std::string& b);

#endif
