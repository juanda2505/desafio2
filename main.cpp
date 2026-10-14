#include <iostream>
#include <fstream>
#include <string>
#include "usuario.h"

// 1000 porque el enunciado exige la demo con mínimo 1000 usuarios registrados
// y el archivo generado tiene exactamente esa cantidad.
const int MAX_USUARIOS = 1000;

int main(int argc, char* argv[]) {
    std::string ruta = (argc > 1) ? argv[1] : "datos/usuarios.txt";

    // ---------- Bloque 3: carga ----------
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir " << ruta << "\n";
        return 1;
    }

    Usuario* usuarios = new Usuario[MAX_USUARIOS];
    int cargados = 0;
    std::string linea;
    std::getline(archivo, linea);   // la primera línea es el encabezado: se descarta

    while (cargados < MAX_USUARIOS && std::getline(archivo, linea)) {
        // Las líneas vacías o mal formadas se saltan.
        if (usuarios[cargados].cargarDesdeLinea(linea, cargados)) {
            cargados++;
        }
    }
    archivo.close();

    std::cout << "Usuarios cargados: " << cargados << "\n";
    if (cargados > 0) {
        std::cout << "Primero: ";
        usuarios[0].mostrar();
        std::cout << "Ultimo:  ";
        usuarios[cargados - 1].mostrar();
    }

    // ---------- Bloque 4: login ----------
    std::string nombreIngresado, contrasenaIngresada;
    std::cout << "\nUsuario: ";
    std::cin >> nombreIngresado;
    std::cout << "Contrasena: ";
    std::cin >> contrasenaIngresada;

    int encontrado = -1;
    for (int i = 0; i < cargados && encontrado == -1; i++) {
        if (compararCadenas(usuarios[i].getNombre(), nombreIngresado)) {
            encontrado = i;
        }
    }

    if (encontrado == -1) {
        std::cout << "Usuario no encontrado.\n";
    } else if (!compararCadenas(usuarios[encontrado].getContrasena(), contrasenaIngresada)) {
        std::cout << "Contrasena incorrecta.\n";
    } else {
        std::cout << "Bienvenido, " << usuarios[encontrado].getNombre() << "!\n";
    }

    delete[] usuarios;
    return 0;
}
