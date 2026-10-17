// clase con metodos estaticos para leer y comparar texto
// no guarda estado: se usa como Archivo::metodo(...), sin crear objetos.
// todos los metodos usan ciclos propios (sin find, substr ni stoi)

#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>

class Archivo {
public:
    // separa una linea por comas en 'campos'. devuelve true solo si la linea
    // tiene exactamente 'esperados' campos. ignora el '\r' de windows
    static bool separarCampos(const std::string& linea, std::string campos[], int esperados);

    // igual que separarCampos, pero el ultimo campo se queda con el resto de la
    // linea aunque tenga comas (se usa para el texto de los mensajes)
    static bool separarCamposConResto(const std::string& linea, std::string campos[], int esperados);

    // convierte texto a entero positivo.
    // devuelve false si el texto esta vacio o tiene algo que no sea digito
    static bool textoAEntero(const std::string& texto, int& resultado);

    // cuenta las lineas de datos de un archivo (sin el encabezado).
    // devuelve -1 si no se pudo abrir
    static int contarLineasDeDatos(const std::string& ruta);

    // compara dos cadenas caracter por caracter
    static bool compararCadenas(const std::string& a, const std::string& b);

    // true si a va antes que b en orden alfabetico (para ordenar nombres)
    static bool esMenor(const std::string& a, const std::string& b);
};

#endif
