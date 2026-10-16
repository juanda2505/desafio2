// implementacion de la clase red social

#include "redsocial.h"
#include "archivo.h"
#include <iostream>
#include <fstream>

// 1000 porque el enunciado exige la demo con minimo 1000 usuarios registrados
// y el archivo generado tiene exactamente esa cantidad
const int MAX_USUARIOS = 1000;

RedSocial::RedSocial(const std::string& carpetaDatos)
    : carpeta(carpetaDatos), usuarios(nullptr), cantidadUsuarios(0), ordenPorNombre(nullptr),
      albumes(nullptr), cantidadAlbumes(0), fotos(nullptr), cantidadFotos(0),
      cambiosAmistades(false), cambiosSolicitudes(false) {}

// liberacion de memoria
RedSocial::~RedSocial() {
    delete[] fotos;
    delete[] albumes;
    delete[] ordenPorNombre;
    delete[] usuarios;
}

// carga de todos los archivos, en orden: primero usuarios, porque los demas
// archivos se vinculan con ellos por posicion
bool RedSocial::cargarDatos() {
    if (!cargarUsuarios()) return false;
    construirIndiceNombres();
    if (!cargarAlbumes() || !cargarFotos() || !cargarAmistades()) return false;
    cargarSolicitudes();
    return true;
}

// lectura de usuarios
bool RedSocial::cargarUsuarios() {
    std::ifstream archivo(carpeta + "usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir " << carpeta << "usuarios.txt\n";
        return false;
    }
    usuarios = new Usuario[MAX_USUARIOS];
    std::string linea;
    std::getline(archivo, linea);   // la primera linea es el encabezado: se descarta

    // cada linea valida se guarda en la siguiente posicion libre;
    // las lineas vacias o mal formadas se saltan
    while (cantidadUsuarios < MAX_USUARIOS && std::getline(archivo, linea)) {
        if (usuarios[cantidadUsuarios].cargarDesdeLinea(linea, cantidadUsuarios)) {
            cantidadUsuarios++;
        }
    }
    std::cout << "Usuarios cargados: " << cantidadUsuarios << "\n";
    return true;
}

// indice ordenado por nombre con ordenamiento por mezcla (merge sort): o(n log n).
// asi cada busqueda por nombre es binaria, o(log n), unas 10 comparaciones
// en vez de recorrer hasta 1000 usuarios
void RedSocial::construirIndiceNombres() {
    int n = cantidadUsuarios;
    ordenPorNombre = new int[n];
    int* auxiliar = new int[n];
    for (int i = 0; i < n; i++) ordenPorNombre[i] = i;

    // se mezclan tramos de tamano 1, 2, 4, ... hasta cubrir todo el arreglo
    for (int ancho = 1; ancho < n; ancho *= 2) {
        for (int ini = 0; ini < n; ini += 2 * ancho) {
            int medio = (ini + ancho < n) ? ini + ancho : n;
            int fin   = (ini + 2 * ancho < n) ? ini + 2 * ancho : n;
            int i = ini, j = medio, k = ini;
            while (i < medio && j < fin) {
                if (Archivo::esMenor(usuarios[ordenPorNombre[j]].getNombre(),
                                         usuarios[ordenPorNombre[i]].getNombre()))
                    auxiliar[k++] = ordenPorNombre[j++];
                else
                    auxiliar[k++] = ordenPorNombre[i++];
            }
            while (i < medio) auxiliar[k++] = ordenPorNombre[i++];
            while (j < fin)   auxiliar[k++] = ordenPorNombre[j++];
        }
        for (int i = 0; i < n; i++) ordenPorNombre[i] = auxiliar[i];
    }
    delete[] auxiliar;
}

int RedSocial::buscarUsuario(const std::string& nombre) const {
    int ini = 0, fin = cantidadUsuarios - 1;
    while (ini <= fin) {
        int medio = (ini + fin) / 2;
        const std::string& actual = usuarios[ordenPorNombre[medio]].getNombre();
        if (Archivo::compararCadenas(actual, nombre)) return ordenPorNombre[medio];
        if (Archivo::esMenor(actual, nombre)) ini = medio + 1;
        else fin = medio - 1;
    }
    return -1;
}

// busca el album 'titulo' del usuario 'idUsuario' y devuelve su posicion, o -1.
// 'pista' es la posicion donde se encontro el anterior: como fotos.txt esta
// ordenado por album, casi siempre es ese mismo o el siguiente
int RedSocial::buscarAlbum(int idUsuario, const std::string& titulo, int& pista) const {
    for (int d = 0; d <= 1; d++) {
        int i = pista + d;
        if (i >= 0 && i < cantidadAlbumes && albumes[i].getIdPropietario() == idUsuario
            && Archivo::compararCadenas(albumes[i].getTitulo(), titulo)) {
            pista = i;
            return i;
        }
    }
    for (int i = 0; i < cantidadAlbumes; i++) {   // si la pista falla, busqueda completa
        if (albumes[i].getIdPropietario() == idUsuario
            && Archivo::compararCadenas(albumes[i].getTitulo(), titulo)) {
            pista = i;
            return i;
        }
    }
    return -1;
}

// lectura de albumes.txt: usuario,album,ruta,privacidad
// se cuentan primero las lineas para reservar el tamano exacto
bool RedSocial::cargarAlbumes() {
    int total = Archivo::contarLineasDeDatos(carpeta + "albumes.txt");
    if (total < 0) {
        std::cout << "No se pudo abrir albumes.txt\n";
        return false;
    }
    albumes = new Album[total];
    std::ifstream archivo(carpeta + "albumes.txt");
    std::string linea, campo[4];
    std::getline(archivo, linea);                         // encabezado
    while (cantidadAlbumes < total && std::getline(archivo, linea)) {
        if (!Archivo::separarCampos(linea, campo, 4)) continue;
        int idU = buscarUsuario(campo[0]);                // nombre -> posicion
        if (albumes[cantidadAlbumes].cargar(idU, campo[1], campo[2], campo[3])) {
            cantidadAlbumes++;
        }
    }
    std::cout << "Albumes cargados: " << cantidadAlbumes << "\n";
    return true;
}

// lectura de fotos.txt: usuario,album,foto,fecha_captura,tamano_kb,ruta,likes
bool RedSocial::cargarFotos() {
    int total = Archivo::contarLineasDeDatos(carpeta + "fotos.txt");
    if (total < 0) {
        std::cout << "No se pudo abrir fotos.txt\n";
        return false;
    }
    fotos = new Foto[total];
    std::ifstream archivo(carpeta + "fotos.txt");
    std::string linea, campo[7];
    int pistaAlbum = 0, sinAlbum = 0;
    std::getline(archivo, linea);                         // encabezado
    while (cantidadFotos < total && std::getline(archivo, linea)) {
        if (!Archivo::separarCampos(linea, campo, 7)) continue;
        // se busca primero el usuario y luego su album
        int idU = buscarUsuario(campo[0]);
        int idA = (idU < 0) ? -1 : buscarAlbum(idU, campo[1], pistaAlbum);
        if (idA < 0) { sinAlbum++; continue; }
        if (fotos[cantidadFotos].cargar(idA, campo[2], campo[3], campo[4], campo[5], campo[6])
            && albumes[idA].agregarFoto(cantidadFotos)) {
            cantidadFotos++;
        }
    }
    std::cout << "Fotos cargadas: " << cantidadFotos << "\n";
    if (sinAlbum > 0) std::cout << "Fotos sin album valido: " << sinAlbum << "\n";
    return true;
}

// lectura de amistades.txt: usuario1,usuario2
// cada amistad aparece una sola vez y se registra en los dos usuarios
bool RedSocial::cargarAmistades() {
    std::ifstream archivo(carpeta + "amistades.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir amistades.txt\n";
        return false;
    }
    std::string linea, campo[2];
    int total = 0;
    std::getline(archivo, linea);                         // encabezado
    while (std::getline(archivo, linea)) {
        if (!Archivo::separarCampos(linea, campo, 2)) continue;
        int a = buscarUsuario(campo[0]);
        int b = buscarUsuario(campo[1]);
        if (a < 0 || b < 0 || a == b) continue;
        usuarios[a].agregarAmigo(b);
        usuarios[b].agregarAmigo(a);
        total++;
    }
    std::cout << "Amistades cargadas: " << total << "\n";
    return true;
}

// lectura de solicitudes.txt: emisor,receptor,fecha,estado
// si el archivo no existe todavia, simplemente no hay solicitudes
void RedSocial::cargarSolicitudes() {
    std::ifstream archivo(carpeta + "solicitudes.txt");
    if (!archivo.is_open()) {
        std::cout << "Solicitudes cargadas: 0\n";
        return;
    }
    std::string linea, campo[4];
    int total = 0;
    std::getline(archivo, linea);                         // encabezado
    while (std::getline(archivo, linea)) {
        if (!Archivo::separarCampos(linea, campo, 4)) continue;
        int emisor = buscarUsuario(campo[0]);
        int receptor = buscarUsuario(campo[1]);
        EstadoSolicitud estado;
        if (emisor < 0 || receptor < 0 || !SolicitudAmistad::estadoDesdeTexto(campo[3], estado)) continue;
        usuarios[receptor].recibirSolicitud(
            SolicitudAmistad(emisor, receptor, Fecha::desdeTexto(campo[2]), estado));
        total++;
    }
    std::cout << "Solicitudes cargadas: " << total << "\n";
}

// guardado: solo se reescribe lo que cambio
void RedSocial::guardarDatos() const {
    if (cambiosAmistades)   guardarAmistades();
    if (cambiosSolicitudes) guardarSolicitudes();
}

// cada amistad se escribe una sola vez: solo cuando el amigo tiene posicion mayor
void RedSocial::guardarAmistades() const {
    std::ofstream archivo(carpeta + "amistades.txt");
    archivo << "usuario1,usuario2\n";
    for (int i = 0; i < cantidadUsuarios; i++) {
        const Arreglo<int>& amigos = usuarios[i].obtenerAmigos();
        for (int k = 0; k < amigos.tamano(); k++) {
            if (amigos[k] > i) {
                archivo << usuarios[i].getNombre() << "," << usuarios[amigos[k]].getNombre() << "\n";
            }
        }
    }
    std::cout << "Amistades guardadas en " << carpeta << "amistades.txt\n";
}

void RedSocial::guardarSolicitudes() const {
    std::ofstream archivo(carpeta + "solicitudes.txt");
    archivo << "emisor,receptor,fecha,estado\n";
    for (int i = 0; i < cantidadUsuarios; i++) {
        const Arreglo<SolicitudAmistad>& lista = usuarios[i].getSolicitudesRecibidas();
        for (int k = 0; k < lista.tamano(); k++) {
            archivo << usuarios[lista[k].getIdEmisor()].getNombre() << ","
                    << usuarios[lista[k].getIdReceptor()].getNombre() << ","
                    << lista[k].getFechaCreacion() << "," << lista[k].estadoTexto() << "\n";
        }
    }
    std::cout << "Solicitudes guardadas en " << carpeta << "solicitudes.txt\n";
}

// menu

// lee una opcion numerica; devuelve -1 si no es un numero valido
// y FIN_ENTRADA si se acabo la entrada, para salir sin quedar en un ciclo infinito
int RedSocial::leerOpcion() {
    std::string texto;
    if (!(std::cin >> texto)) return FIN_ENTRADA;
    int valor;
    if (!Archivo::textoAEntero(texto, valor)) return -1;
    return valor;
}

// en todos los menus la opcion de salir es el numero siguiente a la ultima opcion
void RedSocial::ejecutar() {
    const int SALIR = 2;
    int opcion;
    do {
        std::cout << "\nUde@Book\n"
                  << "1. Iniciar sesion\n"
                  << "2. Salir\n"
                  << "Opcion: ";
        opcion = leerOpcion();
        if (opcion == 1) {
            int id = iniciarSesion();
            if (id >= 0) menuPrincipal(id);
        } else if (opcion != SALIR && opcion != FIN_ENTRADA) {
            std::cout << "Opcion no valida.\n";
        }
    } while (opcion != SALIR && opcion != FIN_ENTRADA);
    guardarDatos();
}

// inicio de sesion: la contrasena solo se compara si el usuario existe
int RedSocial::iniciarSesion() {
    std::string nombre, contrasena;
    std::cout << "Usuario: ";
    std::cin >> nombre;
    std::cout << "Contrasena: ";
    std::cin >> contrasena;

    int id = buscarUsuario(nombre);
    if (id == -1) {
        std::cout << "Usuario no encontrado.\n";
        return -1;
    }
    if (!Archivo::compararCadenas(usuarios[id].getContrasena(), contrasena)) {
        std::cout << "Contrasena incorrecta.\n";
        return -1;
    }
    std::cout << "Bienvenido, " << usuarios[id].getNombre() << "!\n";
    return id;
}

void RedSocial::menuPrincipal(int idActual) {
    const int CERRAR_SESION = 2;
    int opcion;
    do {
        std::cout << "\nMenu principal (" << usuarios[idActual].getNombre() << ")\n"
                  << "1. Administrar amistades\n"
                  << "2. Cerrar sesion\n"
                  << "Opcion: ";
        opcion = leerOpcion();
        if (opcion == 1) menuAmistades(idActual);
        else if (opcion != CERRAR_SESION && opcion != FIN_ENTRADA) std::cout << "Opcion no valida.\n";
    } while (opcion != CERRAR_SESION && opcion != FIN_ENTRADA);
}

// administrar amistades
void RedSocial::menuAmistades(int idActual) {
    const int VOLVER = 3;
    int opcion;
    do {
        std::cout << "\nAdministrar amistades\n"
                  << "1. Enviar solicitud de amistad\n"
                  << "2. Ver y aceptar solicitudes recibidas\n"
                  << "3. Volver\n"
                  << "Opcion: ";
        opcion = leerOpcion();
        if (opcion == 1) enviarSolicitud(idActual);
        else if (opcion == 2) verSolicitudesRecibidas(idActual);
        else if (opcion != VOLVER && opcion != FIN_ENTRADA) std::cout << "Opcion no valida.\n";
    } while (opcion != VOLVER && opcion != FIN_ENTRADA);
}

// enviar solicitud: primero se verifica que el usuario exista,
// las demas verificaciones las hace la clase usuario
void RedSocial::enviarSolicitud(int idActual) {
    std::string nombre;
    std::cout << "Nombre del usuario: ";
    std::cin >> nombre;

    int idDestino = buscarUsuario(nombre);
    if (idDestino == -1) {
        std::cout << "El usuario " << nombre << " no existe.\n";
        return;
    }
    int resultado = usuarios[idActual].enviarSolicitud(usuarios[idDestino], Fecha::ahora());
    switch (resultado) {
        case 0:
            std::cout << "Solicitud enviada a " << nombre << ".\n";
            cambiosSolicitudes = true;
            break;
        case 1: std::cout << "No puedes enviarte una solicitud a ti mismo.\n"; break;
        case 2: std::cout << nombre << " ya es tu amigo.\n"; break;
        case 3: std::cout << "Ya le enviaste una solicitud a " << nombre << " y sigue pendiente.\n"; break;
        case 4: std::cout << nombre << " ya te envio una solicitud. Aceptala en la opcion 2.\n"; break;
    }
}

// ver solicitudes recibidas: solo se muestran las pendientes, numeradas desde 1.
// 'posiciones' guarda en que lugar de la lista esta cada una
void RedSocial::verSolicitudesRecibidas(int idActual) {
    const Arreglo<SolicitudAmistad>& lista = usuarios[idActual].getSolicitudesRecibidas();
    Arreglo<int> posiciones;
    for (int k = 0; k < lista.tamano(); k++) {
        if (lista[k].estaPendiente()) {
            posiciones.agregar(k);
            std::cout << posiciones.tamano() << ". "
                      << usuarios[lista[k].getIdEmisor()].getNombre()
                      << " (" << lista[k].getFechaCreacion() << ")\n";
        }
    }
    if (posiciones.vacia()) {
        std::cout << "No tienes solicitudes pendientes.\n";
        return;
    }
    // la opcion de volver va justo despues de la ultima solicitud
    int volver = posiciones.tamano() + 1;
    std::cout << volver << ". Volver\n"
              << "Numero de la solicitud: ";
    int numero = leerOpcion();
    if (numero == volver || numero == FIN_ENTRADA) return;
    if (numero < 1 || numero > posiciones.tamano()) {
        std::cout << "Numero no valido.\n";
        return;
    }
    int posicion = posiciones[numero - 1];
    int idEmisor = lista[posicion].getIdEmisor();

    std::cout << "1. Aceptar\n2. Rechazar\n3. Volver\nOpcion: ";
    int accion = leerOpcion();
    if (accion == 3 || accion == FIN_ENTRADA) return;
    if (accion == 1) {
        usuarios[idActual].aceptarSolicitud(posicion);
        // la amistad es mutua: se registra en los dos usuarios
        usuarios[idActual].agregarAmigo(idEmisor);
        usuarios[idEmisor].agregarAmigo(idActual);
        std::cout << "Ahora tu y " << usuarios[idEmisor].getNombre() << " son amigos.\n";
        cambiosAmistades = true;
        cambiosSolicitudes = true;
    } else if (accion == 2) {
        usuarios[idActual].rechazarSolicitud(posicion);
        std::cout << "Solicitud de " << usuarios[idEmisor].getNombre() << " rechazada.\n";
        cambiosSolicitudes = true;
    } else {
        std::cout << "Opcion no valida.\n";
    }
}
