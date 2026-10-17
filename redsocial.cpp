// implementacion de la clase red social

#include "redsocial.h"
#include "archivo.h"
#include "metricas.h"
#include <iostream>
#include <fstream>

// 1000 porque el enunciado exige la demo con minimo 1000 usuarios registrados
// y el archivo generado tiene exactamente esa cantidad
const int MAX_USUARIOS = 1000;

RedSocial::RedSocial(const std::string& carpetaDatos)
    : carpeta(carpetaDatos), usuarios(nullptr), cantidadUsuarios(0), ordenPorNombre(nullptr),
      albumes(nullptr), cantidadAlbumes(0), capacidadAlbumes(0),
      fotos(nullptr), cantidadFotos(0), capacidadFotos(0),
      cambiosAmistades(false), cambiosSolicitudes(false), cambiosLikes(false),
      generador(std::random_device{}()) {}

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
    cargarLikes();
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
        Metricas::contar();
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
    capacidadAlbumes = total;
    std::ifstream archivo(carpeta + "albumes.txt");
    std::string linea, campo[4];
    std::getline(archivo, linea);                         // encabezado
    while (cantidadAlbumes < total && std::getline(archivo, linea)) {
        if (!Archivo::separarCampos(linea, campo, 4)) continue;
        int idU = buscarUsuario(campo[0]);                // nombre -> posicion
        // el album tambien queda registrado en su dueno
        if (albumes[cantidadAlbumes].cargar(idU, campo[1], campo[2], campo[3])
            && usuarios[idU].agregarAlbum(cantidadAlbumes)) {
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
    capacidadFotos = total;
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

// lectura de likes.txt: usuario,ruta_foto
// guarda quien dio like a cada foto dentro de la plataforma.
// la ruta identifica la foto de forma unica. si el archivo no existe, no hay likes
void RedSocial::cargarLikes() {
    std::ifstream archivo(carpeta + "likes.txt");
    if (!archivo.is_open()) {
        std::cout << "Likes cargados: 0\n";
        return;
    }
    std::string linea, campo[2];
    int total = 0;
    std::getline(archivo, linea);                         // encabezado
    while (std::getline(archivo, linea)) {
        if (!Archivo::separarCampos(linea, campo, 2)) continue;
        int idU = buscarUsuario(campo[0]);
        if (idU < 0) continue;
        for (int f = 0; f < cantidadFotos; f++) {
            if (Archivo::compararCadenas(fotos[f].getRuta(), campo[1])) {
                fotos[f].agregarLike(idU);
                total++;
                break;
            }
        }
    }
    std::cout << "Likes cargados: " << total << "\n";
}

// guardado: solo se reescribe lo que cambio.
// el indicador solo vuelve a falso si el archivo se escribio bien;
// si fallo, se intentara de nuevo en el siguiente guardado
void RedSocial::guardarDatos() {
    if (cambiosAmistades && guardarAmistades())     cambiosAmistades = false;
    if (cambiosSolicitudes && guardarSolicitudes()) cambiosSolicitudes = false;
    if (cambiosLikes && guardarLikes())             cambiosLikes = false;
}

bool RedSocial::guardarLikes() const {
    std::ofstream archivo(carpeta + "likes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo guardar " << carpeta << "likes.txt\n";
        return false;
    }
    archivo << "usuario,ruta_foto\n";
    for (int f = 0; f < cantidadFotos; f++) {
        const Arreglo<int>& quienes = fotos[f].getUsuariosLike();
        for (int k = 0; k < quienes.tamano(); k++) {
            archivo << usuarios[quienes[k]].getNombre() << "," << fotos[f].getRuta() << "\n";
        }
    }
    std::cout << "Likes guardados en " << carpeta << "likes.txt\n";
    return true;
}

// memoria de todo el sistema: los arreglos reservados (tamano del objeto por
// capacidad) mas lo que cada objeto tiene en el heap (textos y listas)
long long RedSocial::memoriaTotal() const {
    long long total = sizeof(RedSocial) + Metricas::memoriaTexto(carpeta);
    if (usuarios) total += (long long)sizeof(Usuario) * MAX_USUARIOS;
    if (ordenPorNombre) total += (long long)sizeof(int) * cantidadUsuarios;
    total += (long long)sizeof(Album) * capacidadAlbumes;
    total += (long long)sizeof(Foto) * capacidadFotos;
    for (int i = 0; i < cantidadUsuarios; i++) total += usuarios[i].memoria();
    for (int i = 0; i < cantidadAlbumes; i++)  total += albumes[i].memoria();
    for (int i = 0; i < cantidadFotos; i++)    total += fotos[i].memoria();
    return total;
}

// cada amistad se escribe una sola vez: solo cuando el amigo tiene posicion mayor
bool RedSocial::guardarAmistades() const {
    std::ofstream archivo(carpeta + "amistades.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo guardar " << carpeta << "amistades.txt\n";
        return false;
    }
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
    return true;
}

bool RedSocial::guardarSolicitudes() const {
    std::ofstream archivo(carpeta + "solicitudes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo guardar " << carpeta << "solicitudes.txt\n";
        return false;
    }
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
    return true;
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
    // al salir normalmente ya no queda nada pendiente, pero se revisa por seguridad
    guardarDatos();
}

// inicio de sesion: la contrasena solo se compara si el usuario existe
int RedSocial::iniciarSesion() {
    Metricas::reiniciar();
    std::string nombre, contrasena;
    std::cout << "Usuario: ";
    std::cin >> nombre;
    std::cout << "Contrasena: ";
    std::cin >> contrasena;

    int id = buscarUsuario(nombre);
    // memoria de las variables locales de esta funcionalidad
    long long local = sizeof(nombre) + sizeof(contrasena) + sizeof(id)
                    + Metricas::memoriaTexto(nombre) + Metricas::memoriaTexto(contrasena);
    if (id == -1) {
        std::cout << "Usuario no encontrado.\n";
        id = -1;
    } else if (!Archivo::compararCadenas(usuarios[id].getContrasena(), contrasena)) {
        std::cout << "Contrasena incorrecta.\n";
        id = -1;
    } else {
        std::cout << "Bienvenido, " << usuarios[id].getNombre() << "!\n";
    }
    Metricas::mostrar("inicio de sesion", memoriaTotal() + local);
    return id;
}

void RedSocial::menuPrincipal(int idActual) {
    // al entrar se muestran tres fotos de perfil de amigos.
    // 'comunes' se calcula una sola vez y se reutiliza si el usuario actualiza
    Metricas::reiniciar();
    Arreglo<int> comunes, mostrados;
    contarAmigosEnComun(idActual, comunes);
    mostrarFotosAmigos(idActual, comunes, mostrados, "fotos de perfil de amigos");

    // indicador de la actualizacion: solo se puede usar una vez por sesion
    bool fotosActualizadas = false;

    const int CERRAR_SESION = 4;
    int opcion;
    do {
        std::cout << "\nMenu principal (" << usuarios[idActual].getNombre() << ")\n"
                  << "1. Administrar amistades\n"
                  << "2. Visitar perfil de un amigo\n"
                  << "3. Actualizar fotos de amigos\n"
                  << "4. Cerrar sesion\n"
                  << "Opcion: ";
        opcion = leerOpcion();
        if (opcion == 1) menuAmistades(idActual);
        else if (opcion == 2) visitarPerfil(idActual);
        else if (opcion == 3) {
            if (fotosActualizadas) {
                std::cout << "Ya actualizaste las fotos en esta sesion.\n";
            } else {
                Metricas::reiniciar();
                mostrarFotosAmigos(idActual, comunes, mostrados, "actualizar fotos de amigos");
                fotosActualizadas = true;
            }
        }
        else if (opcion != CERRAR_SESION && opcion != FIN_ENTRADA) std::cout << "Opcion no valida.\n";
    } while (opcion != CERRAR_SESION && opcion != FIN_ENTRADA);

    // al cerrar sesion se guardan los cambios de esta sesion (si los hubo),
    // asi un cierre brusco del programa solo pierde lo de la sesion abierta
    guardarDatos();
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
    Metricas::reiniciar();
    std::string nombre;
    std::cout << "Nombre del usuario: ";
    std::cin >> nombre;

    int idDestino = buscarUsuario(nombre);
    int resultado = -1;
    if (idDestino == -1) std::cout << "El usuario " << nombre << " no existe.\n";
    else resultado = usuarios[idActual].enviarSolicitud(usuarios[idDestino], Fecha::ahora());
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
    long long local = sizeof(nombre) + Metricas::memoriaTexto(nombre)
                    + sizeof(idDestino) + sizeof(resultado) + sizeof(idActual);
    Metricas::mostrar("enviar solicitud", memoriaTotal() + local);
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

// visitar perfil de un amigo
// valida en orden: que exista, que no sea el mismo usuario y que sean amigos.
// luego lista solo los albumes visibles para el visitante
void RedSocial::visitarPerfil(int idActual) {
    Metricas::reiniciar();
    std::string nombre;
    std::cout << "Nombre de tu amigo: ";
    std::cin >> nombre;

    int idAmigo = buscarUsuario(nombre);
    Arreglo<int> visibles;               // posiciones de los albumes que se pueden ver
    if (idAmigo == -1) {
        std::cout << "El usuario " << nombre << " no existe.\n";
    } else if (idAmigo == idActual) {
        std::cout << "Ese es tu propio usuario.\n";
    } else if (!usuarios[idActual].esAmigo(idAmigo)) {
        std::cout << nombre << " no es tu amigo.\n";
    } else {
        // solo se recorren los albumes de ese usuario, no los 1527
        const Usuario& amigo = usuarios[idAmigo];
        for (int k = 0; k < amigo.getCantidadAlbumes(); k++) {
            Metricas::contar();
            int a = amigo.getPrimerAlbum() + k;
            if (albumes[a].esVisiblePara(idActual, true)) visibles.agregar(a);
        }
        if (visibles.vacia()) {
            std::cout << nombre << " no tiene albumes visibles para ti.\n";
        } else {
            int volver = visibles.tamano() + 1;
            int opcion;
            do {
                std::cout << "\nAlbumes de " << nombre << "\n";
                for (int k = 0; k < visibles.tamano(); k++) {
                    Metricas::contar();
                    const Album& al = albumes[visibles[k]];
                    std::cout << k + 1 << ". " << al.getTitulo() << " (" << al.privacidadTexto()
                              << ", " << al.getCantidadFotos() << " fotos)\n";
                }
                std::cout << volver << ". Volver\nOpcion: ";
                opcion = leerOpcion();
                if (opcion >= 1 && opcion <= visibles.tamano()) carrusel(idActual, visibles[opcion - 1]);
                else if (opcion != volver && opcion != FIN_ENTRADA) std::cout << "Opcion no valida.\n";
            } while (opcion != volver && opcion != FIN_ENTRADA);
        }
    }
    long long local = sizeof(nombre) + Metricas::memoriaTexto(nombre) + sizeof(idAmigo)
                    + sizeof(visibles) + visibles.memoria() + sizeof(idActual);
    Metricas::mostrar("visitar perfil de un amigo", memoriaTotal() + local);
}

void RedSocial::mostrarFoto(int idActual, int idFoto, int numero, int total) const {
    const Foto& f = fotos[idFoto];
    std::cout << "\nFoto " << numero << " de " << total << ": " << f.getNombre() << "\n"
              << "  ruta:    " << f.getRuta() << "\n"
              << "  fecha:   " << f.getFechaCaptura().aTextoFecha() << "\n"
              << "  tamano:  " << f.getTamanoKB() << " KB\n"
              << "  likes:   " << f.getLikes()
              << (f.tieneLikeDe(idActual) ? " (te gusta)" : "") << "\n";
}

// carrusel circular: la posicion avanza con (i + 1) % n y retrocede con (i - 1 + n) % n,
// asi despues de la ultima foto sigue la primera y antes de la primera va la ultima
void RedSocial::carrusel(int idActual, int idAlbum) {
    const Album& album = albumes[idAlbum];
    int n = album.getCantidadFotos();
    if (n == 0) {
        std::cout << "El album " << album.getTitulo() << " no tiene fotos.\n";
        return;
    }
    int i = 0;
    int opcion;
    do {
        Metricas::contar();
        int idFoto = album.getPrimeraFoto() + i;
        mostrarFoto(idActual, idFoto, i + 1, n);
        std::cout << "1. Siguiente\n2. Anterior\n"
                  << (fotos[idFoto].tieneLikeDe(idActual) ? "3. Quitar like\n" : "3. Dar like\n")
                  << "4. Salir del album\nOpcion: ";
        opcion = leerOpcion();
        if (opcion == 1) {
            i = (i + 1) % n;
        } else if (opcion == 2) {
            i = (i - 1 + n) % n;
        } else if (opcion == 3) {
            bool quedoConLike = fotos[idFoto].alternarLike(idActual);
            std::cout << (quedoConLike ? "Like agregado. " : "Like quitado. ")
                      << "Ahora tiene " << fotos[idFoto].getLikes() << " likes.\n";
            cambiosLikes = true;
        } else if (opcion != 4 && opcion != FIN_ENTRADA) {
            std::cout << "Opcion no valida.\n";
        }
    } while (opcion != 4 && opcion != FIN_ENTRADA);
}

// fotos de perfil de amigos

// peso de cada amigo segun los amigos en comun. el enunciado pide prioridad
// 1, 2.5 y 3.5; se multiplica por 2 para trabajar con enteros: 2, 5 y 7
int RedSocial::pesoPorComunes(int comunes) {
    if (comunes > 20)  return 7;
    if (comunes >= 11) return 5;
    return 2;
}

// cuenta, para cada amigo, cuantos amigos tiene en comun con el usuario actual.
// se marcan en un arreglo de bool los amigos del usuario (acceso directo por
// posicion) y luego se recorren los amigos de cada amigo: o(suma de sus amigos)
// en vez de comparar listas completas una contra otra
void RedSocial::contarAmigosEnComun(int idActual, Arreglo<int>& comunes) {
    const Arreglo<int>& amigos = usuarios[idActual].obtenerAmigos();
    bool* esMiAmigo = new bool[cantidadUsuarios];
    for (int i = 0; i < cantidadUsuarios; i++) { Metricas::contar(); esMiAmigo[i] = false; }
    for (int k = 0; k < amigos.tamano(); k++) { Metricas::contar(); esMiAmigo[amigos[k]] = true; }

    for (int k = 0; k < amigos.tamano(); k++) {
        const Arreglo<int>& deEse = usuarios[amigos[k]].obtenerAmigos();
        int total = 0;
        for (int j = 0; j < deEse.tamano(); j++) {
            Metricas::contar();
            if (esMiAmigo[deEse[j]]) total++;
        }
        comunes.agregar(total);
    }
    delete[] esMiAmigo;
}

// seleccion por ruleta: cada candidato ocupa un tramo del tamano de su peso,
// se sortea un numero entre 0 y la suma de pesos y gana el tramo donde cae.
// el elegido se bloquea para que no se repita dentro del mismo set
void RedSocial::elegirPonderado(const Arreglo<int>& pesos, bool* bloqueado, int cantidad,
                                Arreglo<int>& elegidos) {
    for (int vez = 0; vez < cantidad; vez++) {
        int suma = 0;
        for (int i = 0; i < pesos.tamano(); i++) {
            Metricas::contar();
            if (!bloqueado[i]) suma += pesos[i];
        }
        if (suma == 0) return;                    // ya no quedan candidatos

        std::uniform_int_distribution<int> sorteo(0, suma - 1);
        int numero = sorteo(generador);
        for (int i = 0; i < pesos.tamano(); i++) {
            Metricas::contar();
            if (bloqueado[i]) continue;
            if (numero < pesos[i]) {
                elegidos.agregar(i);
                bloqueado[i] = true;
                break;
            }
            numero -= pesos[i];
        }
    }
}

// muestra hasta tres fotos de perfil de amigos distintos.
// 'anteriores' son los que se mostraron antes: en la actualizacion se evitan
// para que salga otro grupo; si no alcanzan los amigos, se completan con ellos.
// si el usuario tiene menos de tres amigos se muestran los que tenga
void RedSocial::mostrarFotosAmigos(int idActual, const Arreglo<int>& comunes, Arreglo<int>& anteriores,
                                   const char* funcionalidad) {
    const Arreglo<int>& amigos = usuarios[idActual].obtenerAmigos();
    int n = amigos.tamano();
    if (n == 0) {
        std::cout << "\nAun no tienes amigos para mostrar sus fotos de perfil.\n";
        Metricas::mostrar(funcionalidad, memoriaTotal());
        return;
    }
    int cantidad = (n < 3) ? n : 3;

    Arreglo<int> pesos;
    for (int k = 0; k < n; k++) { Metricas::contar(); pesos.agregar(pesoPorComunes(comunes[k])); }

    bool* bloqueado = new bool[n];
    for (int k = 0; k < n; k++) { Metricas::contar(); bloqueado[k] = false; }
    for (int k = 0; k < anteriores.tamano(); k++) { Metricas::contar(); bloqueado[anteriores[k]] = true; }

    Arreglo<int> elegidos;
    elegirPonderado(pesos, bloqueado, cantidad, elegidos);
    // si no hubo suficientes amigos nuevos, se permite repetir los del set anterior
    if (elegidos.tamano() < cantidad) {
        for (int k = 0; k < anteriores.tamano(); k++) { Metricas::contar(); bloqueado[anteriores[k]] = false; }
        for (int k = 0; k < elegidos.tamano(); k++)   { Metricas::contar(); bloqueado[elegidos[k]] = true; }
        elegirPonderado(pesos, bloqueado, cantidad - elegidos.tamano(), elegidos);
    }

    std::cout << "\nFotos de perfil de tus amigos:\n";
    for (int k = 0; k < elegidos.tamano(); k++) {
        Metricas::contar();
        int pos = elegidos[k];
        const Usuario& amigo = usuarios[amigos[pos]];
        std::cout << "  " << amigo.getNombre() << ": " << amigo.getFotoPerfil()
                  << "  (" << comunes[pos] << " amigos en comun)\n";
    }
    anteriores = elegidos;                         // para la siguiente actualizacion

    long long local = sizeof(pesos) + pesos.memoria() + sizeof(elegidos) + elegidos.memoria()
                    + (long long)sizeof(bool) * n + sizeof(n) + sizeof(cantidad);
    delete[] bloqueado;
    Metricas::mostrar(funcionalidad, memoriaTotal() + local);
}
