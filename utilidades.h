#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>

// Separa una línea por comas en 'campos'. Devuelve true solo si la línea
// tiene exactamente 'esperados' campos. Ignora el '\r' de Windows.
bool separarCampos(const std::string& linea, std::string campos[], int esperados);

// Convierte texto a entero positivo con un ciclo propio.
// Devuelve false si el texto está vacío o tiene algo que no sea dígito.
bool textoAEntero(const std::string& texto, int& resultado);

// Cuenta las líneas de datos de un archivo (sin el encabezado).
// Devuelve -1 si no se pudo abrir.
int contarLineasDeDatos(const std::string& ruta);

#endif
