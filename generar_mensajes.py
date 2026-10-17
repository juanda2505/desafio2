# generador de mensajes de ude@book
# lee datos/amistades.txt y crea datos/mensajes.txt solo entre amigos.
# va en un script aparte y con su propia semilla para no alterar los otros datos.
# uso: python generar_mensajes.py

import os
import random
from datetime import datetime, timedelta

rnd = random.Random(45)
CARPETA_DATOS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "datos")
TOTAL = 12000

# los textos no llevan comas: aunque el programa soporta comas en el texto
# (es el ultimo campo), asi el archivo se lee mas facil a mano
frases = ["hola como estas", "nos vemos manana en la u", "ya hiciste el taller de info",
          "que buena foto la de cartagena", "feliz cumpleanos", "me pasas los apuntes",
          "a que hora es el parcial", "vamos a cine el viernes", "gracias por la ayuda",
          "te escribo mas tarde", "viste el partido de anoche", "que tal el fin de semana",
          "ya entregaste el desafio", "nos vemos en la biblioteca", "me encanto el paseo",
          "cuando volvemos a salir", "te mando el link del video", "buen dia",
          "estoy en clase te llamo luego", "que planes para vacaciones"]

INICIO = datetime(2026, 8, 1, 7, 0)    # unas 11 semanas antes del corte
FIN    = datetime(2026, 10, 15, 22, 0)
CORTE_LEIDOS = datetime(2026, 10, 8)   # los mas viejos casi siempre ya se leyeron

# lectura de amistades
pares = []
amigos = {}
with open(os.path.join(CARPETA_DATOS, "amistades.txt"), encoding="utf-8") as f:
    next(f)                            # encabezado
    for linea in f:
        a, b = linea.strip().split(",")
        pares.append((a, b))
        amigos.setdefault(a, []).append(b)
        amigos.setdefault(b, []).append(a)

def fecha_aleatoria():
    segundos = int((FIN - INICIO).total_seconds())
    return INICIO + timedelta(seconds=rnd.randint(0, segundos))

def leido(fecha):
    probabilidad = 0.9 if fecha < CORTE_LEIDOS else 0.3
    return "si" if rnd.random() < probabilidad else "no"

mensajes = []
# mensajes al azar entre amigos, en cualquier direccion
for _ in range(TOTAL):
    a, b = rnd.choice(pares)
    if rnd.random() < 0.5:
        a, b = b, a
    fecha = fecha_aleatoria()
    mensajes.append((fecha, a, b, leido(fecha), rnd.choice(frases)))

# usuario de la demo: maria1 recibe mensajes repartidos en todo el periodo
for _ in range(60):
    fecha = fecha_aleatoria()
    mensajes.append((fecha, rnd.choice(amigos["maria1"]), "maria1", leido(fecha), rnd.choice(frases)))

# ordenados por fecha: asi el programa los carga ya en orden
mensajes.sort(key=lambda m: m[0])

with open(os.path.join(CARPETA_DATOS, "mensajes.txt"), "w", encoding="utf-8") as f:
    f.write("emisor,receptor,fecha,leido,texto\n")
    for fecha, a, b, l, texto in mensajes:
        f.write(f"{a},{b},{fecha.strftime('%Y-%m-%d %H:%M')},{l},{texto}\n")

print(f"Se generaron {len(mensajes)} mensajes en datos/mensajes.txt")
