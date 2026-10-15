# generador de datos de ude@book
# crea usuarios.txt, albumes.txt, fotos.txt y amistades.txt en la carpeta datos/
# uso: python generar_datos.py

import os
import random
from datetime import date, timedelta

random.seed(42)  # misma semilla: siempre genera los mismos datos
N = 1000

# carpeta datos/ junto a este script, sin importar desde donde se ejecute
CARPETA_DATOS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "datos")
SUBRUTA = "fotos"  # sub-ruta fija, la misma que usan las fotos de perfil

nombres = ["juanito", "maria", "carlos", "laura", "andres",
           "sofia", "daniel", "valentina", "mateo", "camila"]
ciudades = [("Medellin", "Colombia"), ("Bogota", "Colombia"),
            ("Cartagena", "Colombia"), ("Cali", "Colombia"),
            ("Bello", "Colombia"), ("Lima", "Peru"), ("Quito", "Ecuador")]
caracteres = "abcdefghijklmnopqrstuvwxyz0123456789"

def fecha_aleatoria(inicio, fin, rnd=random):
    dias = (fin - inicio).days
    return inicio + timedelta(days=rnd.randint(0, dias))

# generacion de usuarios
# guardar usuario e inscripcion en la lista no llama a random,
# asi que la secuencia y usuarios.txt quedan identicos
usuarios = []

with open(os.path.join(CARPETA_DATOS, "usuarios.txt"), "w", encoding="utf-8") as f:
    f.write("usuario,fecha_nacimiento,correo,foto_perfil,contrasena,ciudad,pais,fecha_inscripcion\n")
    for i in range(1, N + 1):
        usuario = f"{random.choice(nombres)}{i}"
        nacimiento = fecha_aleatoria(date(1960, 1, 1), date(2008, 12, 31))
        correo = f"{usuario}@example.com"
        foto = f"/users/{SUBRUTA}/{usuario}/albums/profile.png"
        contrasena = "".join(random.choices(caracteres, k=8))
        ciudad, pais = random.choice(ciudades)
        inscripcion = fecha_aleatoria(date(2020, 1, 1), date(2026, 10, 7))
        f.write(f"{usuario},{nacimiento},{correo},{foto},{contrasena},{ciudad},{pais},{inscripcion}\n")
        usuarios.append((usuario, inscripcion))

print(f"Se generaron {N} usuarios en datos/usuarios.txt")

# generacion de albumes y fotos
# generador aparte con su propia semilla: no toca la secuencia de los usuarios
rnd = random.Random(43)

nombres_album = ["Vacaciones", "Familia", "Amigos", "Cartagena2026", "Universidad",
                 "Cumpleanos", "Navidad", "Mascotas", "Paseo", "Conciertos"]
nombres_foto = ["Playa", "Atardecer", "Grupo", "Selfie", "Paisaje",
                "Cena", "Malecon", "Parque", "Montana", "Ciudad"]
privacidades = ["publico", "privado", "amigos"]
HOY = date(2026, 10, 7)

# usuario de la demo (punto iv del video): 2 albumes visibles con 5 fotos cada uno
DEMO = {
    "laura2": [("Cartagena2026", "amigos", 5), ("Universidad", "publico", 5)],
}

total_albumes = 0
total_fotos = 0

with open(os.path.join(CARPETA_DATOS, "albumes.txt"), "w", encoding="utf-8") as fa, \
     open(os.path.join(CARPETA_DATOS, "fotos.txt"), "w", encoding="utf-8") as ff:
    fa.write("usuario,album,ruta,privacidad\n")
    ff.write("usuario,album,foto,fecha_captura,tamano_kb,ruta,likes\n")

    for usuario, inscripcion in usuarios:
        if usuario in DEMO:
            albumes = DEMO[usuario]
        else:
            # 0 a 3 albumes por usuario, con nombres distintos entre si
            elegidos = rnd.sample(nombres_album, rnd.randint(0, 3))
            albumes = [(a, rnd.choice(privacidades), rnd.randint(0, 6)) for a in elegidos]

        for album, privacidad, cantidad_fotos in albumes:
            ruta_album = f"/users/{SUBRUTA}/{usuario}/albums/{album}/"
            fa.write(f"{usuario},{album},{ruta_album},{privacidad}\n")
            total_albumes += 1

            # nombre base + numero: nunca se repite una foto dentro del album
            for k in range(1, cantidad_fotos + 1):
                foto = f"{rnd.choice(nombres_foto)}{k}"
                captura = fecha_aleatoria(inscripcion, HOY, rnd)  # despues de inscribirse
                tamano = rnd.randint(200, 5000)                   # kb
                likes = rnd.randint(0, 150)
                ff.write(f"{usuario},{album},{foto},{captura},{tamano},{ruta_album}{foto}.png,{likes}\n")
                total_fotos += 1

print(f"Se generaron {total_albumes} albumes en datos/albumes.txt")
print(f"Se generaron {total_fotos} fotos en datos/fotos.txt")

# generacion de amistades
# otro generador aparte, para no alterar usuarios, albumes ni fotos
ra = random.Random(44)
nombres_usuarios = [u for u, _ in usuarios]
posicion = {u: i for i, u in enumerate(nombres_usuarios)}
pares = set()   # cada amistad se guarda una sola vez: (menor, mayor) por posicion

def hacer_amigos(a, b):
    if a != b:
        pares.add((a, b) if posicion[a] < posicion[b] else (b, a))

def grupo_de_amigos(miembros, probabilidad):
    # dentro de un grupo, cada par es amigo con cierta probabilidad.
    # mas probabilidad = mas amigos en comun entre sus miembros
    for x in range(len(miembros)):
        for y in range(x + 1, len(miembros)):
            if ra.random() < probabilidad:
                hacer_amigos(miembros[x], miembros[y])

otros = [u for u in nombres_usuarios if u not in ("maria1", "laura2")]
ra.shuffle(otros)

# tres grupos especiales para la demo de maria1, con densidad distinta,
# para que sus amigos caigan en las tres prioridades (<=10, 11-20, >20 en comun)
grupo_denso = ["laura2"] + otros[0:44]
grupo_medio = otros[44:79]
grupo_ralo  = otros[79:109]
resto       = otros[109:]
grupo_de_amigos(grupo_denso, 0.75)
grupo_de_amigos(grupo_medio, 0.5)
grupo_de_amigos(grupo_ralo, 0.2)

# el resto se reparte en grupos de 20 a 50 usuarios con densidad variable
i = 0
while i < len(resto):
    t = ra.randint(20, 50)
    grupo_de_amigos(resto[i:i + t], ra.uniform(0.15, 0.45))
    i += t

# algunas amistades sueltas entre grupos distintos
for _ in range(1500):
    hacer_amigos(ra.choice(otros), ra.choice(otros))

# maria1 (usuario de la demo de login) y laura2 (perfil a visitar) son amigas
for u in ["laura2"] + ra.sample(grupo_denso[1:], 29):
    hacer_amigos("maria1", u)
for u in ra.sample(grupo_medio, 25):
    hacer_amigos("maria1", u)
for u in ra.sample(grupo_ralo, 12):
    hacer_amigos("maria1", u)

# ordenadas por posicion del primer usuario: asi el programa las lee en orden
ordenadas = sorted(pares, key=lambda p: (posicion[p[0]], posicion[p[1]]))
with open(os.path.join(CARPETA_DATOS, "amistades.txt"), "w", encoding="utf-8") as f:
    f.write("usuario1,usuario2\n")
    for a, b in ordenadas:
        f.write(f"{a},{b}\n")

print(f"Se generaron {len(ordenadas)} amistades en datos/amistades.txt")
