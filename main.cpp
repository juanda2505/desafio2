#include <iostream>
#include <fstream>
#include <string>
#include "usuario.h"
#include "album.h"
#include "foto.h"
#include "utilidades.h"

// 1000 porque el enunciado exige la demo con mínimo 1000 usuarios registrados
// y el archivo generado tiene exactamente esa cantidad.
const int MAX_USUARIOS = 1000;

// Busca un usuario por nombre y devuelve su posición, o -1.
// 'pista' es la posición donde se encontró el anterior: como los archivos están
// ordenados por usuario, casi siempre el buscado es ese mismo o el siguiente,
// y no hace falta recorrer los 1000.
int buscarUsuario(const Usuario* usuarios, int cantidad, const std::string& nombre, int& pista) {
    for (int d = 0; d <= 1; d++) {
        int i = pista + d;
        if (i >= 0 && i < cantidad && compararCadenas(usuarios[i].getNombre(), nombre)) {
            pista = i;
            return i;
        }
    }
    for (int i = 0; i < cantidad; i++) {          // si la pista falla, búsqueda completa
        if (compararCadenas(usuarios[i].getNombre(), nombre)) {
            pista = i;
            return i;
        }
    }
    return -1;
}

// Busca el álbum 'titulo' del usuario 'idUsuario' y devuelve su posición, o -1.
// Usa la misma idea de la pista que buscarUsuario.
int buscarAlbum(const Album* albumes, int cantidad, int idUsuario, const std::string& titulo, int& pista) {
    for (int d = 0; d <= 1; d++) {
        int i = pista + d;
        if (i >= 0 && i < cantidad && albumes[i].getIdPropietario() == idUsuario
            && compararCadenas(albumes[i].getTitulo(), titulo)) {
            pista = i;
            return i;
        }
    }
    for (int i = 0; i < cantidad; i++) {
        if (albumes[i].getIdPropietario() == idUsuario && compararCadenas(albumes[i].getTitulo(), titulo)) {
            pista = i;
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    std::string carpeta = (argc > 1) ? argv[1] : "datos/";

    // ---------- Bloque 3: carga de usuarios ----------
    std::ifstream archivo(carpeta + "usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir " << carpeta << "usuarios.txt\n";
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

    // ---------- Bloque 6: carga de álbumes y fotos ----------
    // Se cuentan primero las líneas para reservar el tamaño exacto:
    // ni se queda corto ni sobra memoria, y no hay que tocar el código
    // si el generador produce otra cantidad.
    int totalAlbumes = contarLineasDeDatos(carpeta + "albumes.txt");
    int totalFotos   = contarLineasDeDatos(carpeta + "fotos.txt");
    if (totalAlbumes < 0 || totalFotos < 0) {
        std::cout << "No se pudo abrir albumes.txt o fotos.txt\n";
        delete[] usuarios;
        return 1;
    }

    Album* albumes = new Album[totalAlbumes];
    Foto*  fotos   = new Foto[totalFotos];
    int albumesCargados = 0, fotosCargadas = 0;
    int pistaUsuario = 0, pistaAlbum = 0;

    // albumes.txt: usuario,album,ruta,privacidad
    std::string ca[4];
    archivo.open(carpeta + "albumes.txt");
    std::getline(archivo, linea);                         // encabezado
    while (albumesCargados < totalAlbumes && std::getline(archivo, linea)) {
        if (!separarCampos(linea, ca, 4)) continue;
        int idU = buscarUsuario(usuarios, cargados, ca[0], pistaUsuario);
        if (albumes[albumesCargados].cargar(idU, ca[1], ca[2], ca[3])) {
            albumesCargados++;
        }
    }
    archivo.close();

    // fotos.txt: usuario,album,foto,fecha_captura,tamano_kb,ruta,likes
    std::string cf[7];
    int fotosSinAlbum = 0;
    archivo.open(carpeta + "fotos.txt");
    std::getline(archivo, linea);                         // encabezado
    pistaUsuario = 0;
    while (fotosCargadas < totalFotos && std::getline(archivo, linea)) {
        if (!separarCampos(linea, cf, 7)) continue;
        int idU = buscarUsuario(usuarios, cargados, cf[0], pistaUsuario);
        int idA = (idU < 0) ? -1 : buscarAlbum(albumes, albumesCargados, idU, cf[1], pistaAlbum);
        if (idA < 0) { fotosSinAlbum++; continue; }
        if (fotos[fotosCargadas].cargar(idA, cf[2], cf[3], cf[4], cf[5], cf[6])
            && albumes[idA].agregarFoto(fotosCargadas)) {
            fotosCargadas++;
        }
    }
    archivo.close();

    std::cout << "Albumes cargados: " << albumesCargados << "\n";
    std::cout << "Fotos cargadas: " << fotosCargadas << "\n";
    if (fotosSinAlbum > 0) std::cout << "Fotos sin album valido: " << fotosSinAlbum << "\n";

    // Verificación del usuario de la demo
    int pistaDemo = 0;
    int idDemo = buscarUsuario(usuarios, cargados, "laura2", pistaDemo);
    if (idDemo >= 0) {
        std::cout << "\nAlbumes de " << usuarios[idDemo].getNombre() << ":\n";
        for (int a = 0; a < albumesCargados; a++) {
            if (albumes[a].getIdPropietario() != idDemo) continue;
            std::cout << "  " << albumes[a].getTitulo() << " (" << albumes[a].privacidadTexto()
                      << ", " << albumes[a].getCantidadFotos() << " fotos)\n";
            for (int k = 0; k < albumes[a].getCantidadFotos(); k++) {
                const Foto& f = fotos[albumes[a].getPrimeraFoto() + k];
                std::cout << "    - " << f.getNombre() << "  " << f.getRuta()
                          << "  likes: " << f.getLikes() << "\n";
            }
        }
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

    delete[] fotos;
    delete[] albumes;
    delete[] usuarios;
    return 0;
}
