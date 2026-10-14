import os
import random
from datetime import date, timedelta

random.seed(42)  # misma semilla: siempre genera los mismos datos
N = 1000

# Carpeta datos/ junto a este script, sin importar desde dónde se ejecute
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

# ===================== Usuarios (sin cambios) =====================
# Guardar usuario e inscripción en la lista NO llama a random,
# así que la secuencia y usuarios.txt quedan idénticos.
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

# ===================== Álbumes y fotos =====================
# Generador aparte con su propia semilla: no toca la secuencia de los usuarios.
rnd = random.Random(43)

nombres_album = ["Vacaciones", "Familia", "Amigos", "Cartagena2026", "Universidad",
                 "Cumpleanos", "Navidad", "Mascotas", "Paseo", "Conciertos"]
nombres_foto = ["Playa", "Atardecer", "Grupo", "Selfie", "Paisaje",
                "Cena", "Malecon", "Parque", "Montana", "Ciudad"]
privacidades = ["publico", "privado", "amigos"]
HOY = date(2026, 10, 7)

# Usuario de la demo (punto IV del video): 2 álbumes visibles con 5 fotos cada uno.
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
            # 0 a 3 álbumes por usuario, con nombres distintos entre sí
            elegidos = rnd.sample(nombres_album, rnd.randint(0, 3))
            albumes = [(a, rnd.choice(privacidades), rnd.randint(0, 6)) for a in elegidos]

        for album, privacidad, cantidad_fotos in albumes:
            ruta_album = f"/users/{SUBRUTA}/{usuario}/albums/{album}/"
            fa.write(f"{usuario},{album},{ruta_album},{privacidad}\n")
            total_albumes += 1

            # Nombre base + número: nunca se repite una foto dentro del álbum
            for k in range(1, cantidad_fotos + 1):
                foto = f"{rnd.choice(nombres_foto)}{k}"
                captura = fecha_aleatoria(inscripcion, HOY, rnd)  # después de inscribirse
                tamano = rnd.randint(200, 5000)                   # KB
                likes = rnd.randint(0, 150)
                ff.write(f"{usuario},{album},{foto},{captura},{tamano},{ruta_album}{foto}.png,{likes}\n")
                total_fotos += 1

print(f"Se generaron {total_albumes} albumes en datos/albumes.txt")
print(f"Se generaron {total_fotos} fotos en datos/fotos.txt")