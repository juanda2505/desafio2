import random
from datetime import date, timedelta

random.seed(42)  # misma semilla: siempre genera los mismos datos
N = 1000

nombres = ["juanito", "maria", "carlos", "laura", "andres",
           "sofia", "daniel", "valentina", "mateo", "camila"]
ciudades = [("Medellin", "Colombia"), ("Bogota", "Colombia"),
            ("Cartagena", "Colombia"), ("Cali", "Colombia"),
            ("Bello", "Colombia"), ("Lima", "Peru"), ("Quito", "Ecuador")]
caracteres = "abcdefghijklmnopqrstuvwxyz0123456789"

def fecha_aleatoria(inicio, fin):
    dias = (fin - inicio).days
    return inicio + timedelta(days=random.randint(0, dias))

with open(r"C:\Users\Usuario\OneDrive\Documentos\UDEA\5SEMESTRE\info2\desafio2\desafio2\Datos\usuarios.txt", "w", encoding="utf-8") as f:
        f.write("usuario,fecha_nacimiento,correo,foto_perfil,contrasena,ciudad,pais,fecha_inscripcion\n")
        for i in range(1, N + 1):
            usuario = f"{random.choice(nombres)}{i}"  # el número hace que sea único
            nacimiento = fecha_aleatoria(date(1960, 1, 1), date(2008, 12, 31))
            correo = f"{usuario}@example.com"
            foto = f"/users/fotos/{usuario}/albums/profile.png"
            contrasena = "".join(random.choices(caracteres, k=8))
            ciudad, pais = random.choice(ciudades)
            inscripcion = fecha_aleatoria(date(2020, 1, 1), date(2026, 10, 7))
            f.write(f"{usuario},{nacimiento},{correo},{foto},{contrasena},{ciudad},{pais},{inscripcion}\n")

print(f"Se generaron {N} usuarios en usuarios.txt")