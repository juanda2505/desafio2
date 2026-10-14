#include <iostream>
#include <fstream>

// Función para limpiar el carácter '\r' al final de una cadena (típico de archivos de Windows)
void limpiarRetornoCarro(char buffer[]) {
    int i = 0;
    while (buffer[i] != '\0') {
        if (buffer[i] == '\r') {
            buffer[i] = '\0'; // Cortamos la cadena si encontramos '\r'
            break;
        }
        i++;
    }
}

// Función auxiliar para procesar y mostrar los 8 campos de una línea de texto
void procesarLineaUsuario(const char* linea) {
    // Matriz para almacenar los 8 campos (máximo 100 caracteres por campo)
    char campos[8][100];

    int numCampo = 0;
    int posCampo = 0;
    int i = 0;

    // Paso 4: Recorrer la línea carácter por carácter
    while (linea[i] != '\0') {
        if (linea[i] == ',') {
            // Cierra el campo actual con el terminador nulo
            campos[numCampo][posCampo] = '\0';
            numCampo++;      // Avanza al siguiente campo
            posCampo = 0;    // Reinicia la posición del nuevo campo
        } else {
            // Copia el carácter al campo actual
            campos[numCampo][posCampo] = linea[i];
            posCampo++;
        }
        i++;
    }

    // Paso 5: Cerrar el último campo después del ciclo
    campos[numCampo][posCampo] = '\0';

    // Limpia el \r si venía de un archivo de Windows
    limpiarRetornoCarro(campos[7]);

    // Nombres de los encabezados según el orden del archivo
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

    // Paso 6: Mostrar los 8 campos con su nombre correspondiente
    for (int k = 0; k < 8; k++) {
        std::cout << encabezados[k] << ": " << campos[k] << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

int main() {
    std::cout << "=== PRUEBA 1: Paso 3 (Línea Fija) ===" << std::endl;
    // Paso 3: Probar primero con una línea fija simulada
    char lineaFija[] = "maria1,1962-03-30,maria1@example.com,/ruta/foto.png,pass123,Medellin,Colombia,2022-06-14\r";
    procesarLineaUsuario(lineaFija);

    std::cout << "\n=== PRUEBA 2: Pasos 2, 4, 5 y 6 (Desde usuarios.txt) ===" << std::endl;

    // Paso 2: Abrir el archivo datos/usuarios.txt
    std::ifstream archivo("datos/usuarios.txt");

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo datos/usuarios.txt" << std::endl;
        return 1;
    }

    char lineaArchivo[1024];

    // Leer la primera línea y descartarla (Encabezado)
    archivo.getline(lineaArchivo, 1024);

    // Leer la primera línea de datos (maria1)
    if (archivo.getline(lineaArchivo, 1024)) {
        std::cout << "[Usuario 1]:" << std::endl;
        procesarLineaUsuario(lineaArchivo);
    }

    // Probar con la segunda línea de datos (laura2) para verificar que no depende de un solo ejemplo
    if (archivo.getline(lineaArchivo, 1024)) {
        std::cout << "[Usuario 2]:" << std::endl;
        procesarLineaUsuario(lineaArchivo);
    }

    archivo.close();
    return 0;
}