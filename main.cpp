// programa principal de ude@book
// crea la red social, carga los datos y abre el menu.
// al salir se guardan en los archivos los cambios de amistades y solicitudes

#include "redsocial.h"

int main(int argc, char* argv[]) {
    // carpeta de los archivos: se puede pasar como argumento, por defecto datos/
    std::string carpeta = (argc > 1) ? argv[1] : "datos/";

    RedSocial red(carpeta);
    if (!red.cargarDatos()) return 1;
    red.ejecutar();
    return 0;
}
