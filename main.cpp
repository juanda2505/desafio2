#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream archivo("datos/usuarios.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    char linea[512];
    int cantidad = 0;
    archivo.getline(linea, 512); // primera línea: encabezado

    while (archivo.getline(linea, 512)) {
        cantidad++;
    }

    cout << "Usuarios leidos: " << cantidad << endl;
    return 0;
}