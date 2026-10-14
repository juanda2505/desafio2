#include <iostream>
#include <fstream>

using namespace std;
// ============================================================
// PROCESAMIENTO DE UNA LÍNEA
// ============================================================

// Recibe una línea del archivo (texto con campos separados por comas),
// la separa en sus 8 campos y los imprime con su nombre.
void procesarLineaUsuario(const char* linea) {
    char campos[8][100];   // 8 campos, cada uno con hasta 99 caracteres + '\0'
    int numCampo = 0;      // índice del campo que se está leyendo (0 a 7)
    int posCampo = 0;      // posición de escritura dentro del campo actual
    int i = 0;             // índice que recorre la línea original

    // --- Separación de campos ---
    while (linea[i] != '\0') {          // recorre la línea hasta su final
        if (linea[i] == ',') {          // si encuentra una coma, termina el campo
            campos[numCampo][posCampo] = '\0'; // cierra el campo actual
            numCampo++;                 // pasa al siguiente campo
            posCampo = 0;               // vuelve a empezar desde la posición 0
        } else {
            // Si no es coma, copia el carácter al campo actual
            campos[numCampo][posCampo] = linea[i];
            posCampo++;
        }
        i++;                            // avanza al siguiente carácter de la línea
    }

    // El último campo no termina en coma, así que se cierra aquí.
    campos[numCampo][posCampo] = '\0';

    // --- Impresión ---
    // Nombres de los campos, en el mismo orden que el archivo
    const char* encabezados[8] = {
        "usuario",
        "fecha_nacimiento",
        "correo",
        "foto_perfil",
        "contrasena",
        "ciudad",
        "pais",
        "fecha_inscripcion"
    };

    for (int k = 0; k < 8; k++) {       // imprime cada campo con su nombre
        cout << encabezados[k] << ": " << campos[k] << endl;
    }
    cout << "----------------------------------------" << endl;
}

// ============================================================
// PROGRAMA PRINCIPAL
// ============================================================

int main() {
    // Abre el archivo desde la carpeta de trabajo (datos/usuarios.txt)
    ifstream archivo("datos/usuarios.txt");
    if (!archivo) {                     // si no se pudo abrir, avisa y termina
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    char linea[512];                    // buffer para una línea del archivo
    int cantidad = 0;                   // cuenta los usuarios leídos

    // 1. Lee la primera línea (encabezado) y la descarta
    archivo.getline(linea, 512);

    // 2. Lee una línea de usuario por iteración hasta el final del archivo
    while (archivo.getline(linea, 512)) {
        cantidad++;

        // Solo muestra en detalle los dos primeros, para verificar
        if (cantidad <= 2) {
            cout << "\n[Usuario " << cantidad << "]:" << endl;
            procesarLineaUsuario(linea);
        }
    }

    cout << "\nTotal de usuarios leidos y procesados: " << cantidad << endl;

    archivo.close();
    return 0;
}