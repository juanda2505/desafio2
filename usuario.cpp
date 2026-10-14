#include "usuario.h"
#include <iostream>

Usuario::Usuario() : idUsuario(-1) {}

bool Usuario::cargarDesdeLinea(const std::string& linea, int id) {
    // Orden de los campos en el archivo, separados por ',':
    // usuario,fecha_nacimiento,correo,foto_perfil,contrasena,ciudad,pais,fecha_inscripcion
    const int CAMPOS = 8;
    std::string campo[CAMPOS];
    int n = 0;

    for (unsigned i = 0; i < linea.size(); i++) {
        char c = linea[i];
        if (c == '\r') continue;             // archivos creados en Windows
        if (c == ',') {
            n++;
            if (n >= CAMPOS) return false;   // sobran campos
        } else {
            campo[n] += c;
        }
    }
    if (n != CAMPOS - 1) return false;       // faltan campos

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

void Usuario::mostrar() const {
    std::cout << "[" << idUsuario << "] " << nombre
              << " | " << correo
              << " | foto: " << fotoPerfil
              << " | nacimiento: " << fechaNacimiento
              << " | " << ciudad << ", " << pais
              << " | inscripcion: " << fechaInscripcion << "\n";
}

bool compararCadenas(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;  // distinto largo: no hace falta recorrer
    for (unsigned i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}
